#include <math.h>
#include "habitability.h"
#include "../../data/constants.h"

/*
 * Kopparapu et al. 2013 Table 3 coefficients for S_eff calculation.
 * S_eff(T*) = S_eff_sun + a*T_s + b*T_s^2 + c*T_s^3 + d*T_s^4
 * where T_s = T_eff - 5780 (solar reference)
 *
 * Order: Recent Venus, Runaway Greenhouse, Max Greenhouse, Early Mars
 */
static const double KOPPARAPU_SEFF_SUN[HZ_LIMIT_COUNT] = {
    1.7763,   /* Recent Venus */
    1.0512,   /* Runaway Greenhouse */
    0.3438,   /* Maximum Greenhouse */
    0.3179    /* Early Mars */
};

static const double KOPPARAPU_A[HZ_LIMIT_COUNT] = {
    1.4335e-4,
    1.3242e-4,
    5.8942e-5,
    5.4513e-5
};

static const double KOPPARAPU_B[HZ_LIMIT_COUNT] = {
    3.3954e-9,
    1.5418e-9,
    1.6558e-9,
    1.5313e-9
};

static const double KOPPARAPU_C[HZ_LIMIT_COUNT] = {
    -7.6345e-12,
    -7.9895e-12,
    -3.0045e-12,
    -2.7786e-12
};

static const double KOPPARAPU_D[HZ_LIMIT_COUNT] = {
    -1.7246e-15,
    -1.8328e-15,
    -5.2983e-16,
    -4.8997e-16
};

/*
 * Effective stellar flux limit at a given boundary.
 * d_hz = sqrt(L/L_sun / S_eff) gives distance in AU.
 */
static double s_eff_at_boundary(int boundary, double t_eff)
{
    double ts = t_eff - 5780.0;
    return KOPPARAPU_SEFF_SUN[boundary]
         + KOPPARAPU_A[boundary] * ts
         + KOPPARAPU_B[boundary] * ts * ts
         + KOPPARAPU_C[boundary] * ts * ts * ts
         + KOPPARAPU_D[boundary] * ts * ts * ts * ts;
}

habitability_t habitability_compute(double stellar_luminosity,
                                     double stellar_teff,
                                     double planet_flux_si,
                                     double planet_distance_au)
{
    habitability_t hz;
    double l_rel = stellar_luminosity / L_SUN;

    hz.planet_flux_rel    = planet_flux_si / EARTH_INSOLATION;
    hz.planet_distance_au = planet_distance_au;

    for (int i = 0; i < HZ_LIMIT_COUNT; i++) {
        hz.s_eff[i]       = s_eff_at_boundary(i, stellar_teff);
        hz.distance_au[i] = sqrt(l_rel / hz.s_eff[i]);
        hz.distance_m[i]  = hz.distance_au[i] * AU;
    }

    /*
     * Conservative HZ: Runaway Greenhouse (inner) to Max Greenhouse (outer)
     * Optimistic HZ:   Recent Venus (inner) to Early Mars (outer)
     */
    hz.in_conservative_hz = (planet_distance_au >= hz.distance_au[HZ_RUNAWAY_GH] &&
                              planet_distance_au <= hz.distance_au[HZ_MAX_GREENHOUSE]);

    hz.in_optimistic_hz   = (planet_distance_au >= hz.distance_au[HZ_RECENT_VENUS] &&
                              planet_distance_au <= hz.distance_au[HZ_EARLY_MARS]);

    hz.hz_center_au = 0.5 * (hz.distance_au[HZ_RUNAWAY_GH]
                            + hz.distance_au[HZ_MAX_GREENHOUSE]);

    hz.hz_width_au  = hz.distance_au[HZ_MAX_GREENHOUSE]
                    - hz.distance_au[HZ_RUNAWAY_GH];

    return hz;
}
