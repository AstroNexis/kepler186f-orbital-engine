#include <math.h>
#include "tidal.h"
#include "../../data/constants.h"

#define PI  3.14159265358979323846

/*
 * Tidal locking timescale (MacDonald 1964 / Barnes 2010):
 *   t_lock = (omega_0 * a^6 * Q * m_p) / (3 * G * M_s^2 * R_p^3 * k2)
 *
 * omega_0: initial rotation rate (assume 1 rad/s, fast rotator proxy)
 * k2: Love number (rigidity-dependent); for rocky body k2 ≈ 0.3
 *
 * This gives an order-of-magnitude estimate; actual value depends
 * on initial spin period and internal structure.
 */
static double tidal_lock_timescale(double semi_major, double planet_mass,
                                    double planet_radius, double stellar_mass,
                                    double Q_factor, double rigidity)
{
    double omega_0 = 1.0;
    double k2 = 1.5 / (1.0 + (19.0 * rigidity) / (2.0 * G * planet_mass
                       / (planet_radius * planet_radius * planet_radius)
                       * planet_radius));

    double a6  = pow(semi_major, 6.0);
    double rp3 = pow(planet_radius, 3.0);
    double ms2 = stellar_mass * stellar_mass;

    double t = (omega_0 * a6 * Q_factor * planet_mass)
             / (3.0 * G * ms2 * rp3 * k2);

    return t / YEAR_SECONDS;
}

/*
 * Tidal heating power (Peale et al. 1979, Io-based formulation):
 *   P_tidal = (21/2) * (k2/Q) * (G*M_s^2 * R_p^5 * n * e^2) / a^6
 *
 * n = mean motion (rad/s)
 * Dominant for non-zero eccentricity; P -> 0 as e -> 0.
 */
static double tidal_heating(double semi_major, double planet_radius,
                             double stellar_mass, double eccentricity,
                             double Q_factor, double rigidity,
                             double mean_motion, double planet_mass)
{
    if (eccentricity < 1e-10)
        return 0.0;

    double k2 = 1.5 / (1.0 + (19.0 * rigidity) / (2.0 * G * planet_mass
                       / (planet_radius * planet_radius * planet_radius)
                       * planet_radius));

    double a6  = pow(semi_major, 6.0);
    double rp5 = pow(planet_radius, 5.0);
    double e2  = eccentricity * eccentricity;

    return (21.0 / 2.0) * (k2 / Q_factor)
         * (G * stellar_mass * stellar_mass * rp5 * mean_motion * e2) / a6;
}

/*
 * Roche limit for a fluid body:
 *   d_R = 2.44 * R_s * (rho_s / rho_p)^(1/3)
 * Rewritten in terms of masses and radii to avoid density lookup.
 */
static double roche_limit(double stellar_mass, double stellar_radius,
                           double planet_mass, double planet_radius)
{
    double rho_s = stellar_mass / ((4.0 / 3.0) * PI * pow(stellar_radius, 3.0));
    double rho_p = planet_mass  / ((4.0 / 3.0) * PI * pow(planet_radius,  3.0));
    return 2.44 * stellar_radius * pow(rho_s / rho_p, 1.0 / 3.0);
}

tidal_t tidal_compute(double planet_mass, double planet_radius,
                      double semi_major, double stellar_mass,
                      double eccentricity, double Q_factor,
                      double rigidity, double stellar_age_s,
                      double orbital_period_s)
{
    tidal_t td;

    double mean_motion = (2.0 * PI) / orbital_period_s;

    td.tidal_lock_timescale_yr = tidal_lock_timescale(semi_major, planet_mass,
                                                       planet_radius, stellar_mass,
                                                       Q_factor, rigidity);
    td.tidal_lock_timescale_gyr = td.tidal_lock_timescale_yr / 1e9;

    double stellar_age_yr = stellar_age_s / YEAR_SECONDS;
    td.is_likely_locked = (td.tidal_lock_timescale_yr < stellar_age_yr) ? 1.0 : 0.0;

    double P_tidal = tidal_heating(semi_major, planet_radius, stellar_mass,
                                    eccentricity, Q_factor, rigidity,
                                    mean_motion, planet_mass);

    td.tidal_heating_power = P_tidal;
    td.tidal_heating_flux  = P_tidal / (4.0 * PI * planet_radius * planet_radius);

    td.sync_rotation_period_s = orbital_period_s;

    /* Use solar radius as proxy for K186 (0.472 R_sun baked into constants) */
    td.roche_limit = roche_limit(stellar_mass, 0.472 * R_SUN,
                                  planet_mass, planet_radius);

    return td;
}
