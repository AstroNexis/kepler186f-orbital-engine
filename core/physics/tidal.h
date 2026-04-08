#ifndef TIDAL_H
#define TIDAL_H

/*
 * Tidal mechanics module.
 * Estimates tidal locking timescale and tidal heating power.
 *
 * References:
 *   Goldreich & Soter 1966 (Q factor)
 *   Barnes et al. 2010 (tidal locking timescale)
 *   Peale 1977 (synchronous rotation)
 */

typedef struct {
    double tidal_lock_timescale_yr;  /* years to reach synchronous rotation */
    double tidal_lock_timescale_gyr; /* same, in Gyr */
    double is_likely_locked;         /* 1 if t_lock < stellar age, else 0 */
    double tidal_heating_power;      /* W, dissipated in planet interior */
    double tidal_heating_flux;       /* W/m^2, surface flux from tidal heating */
    double sync_rotation_period_s;   /* = orbital period if locked */
    double roche_limit;              /* m, fluid body approximation */
} tidal_t;

/*
 * planet_mass_kg, planet_radius_m: planet parameters
 * semi_major_m: orbital distance
 * stellar_mass_kg: host star
 * eccentricity: orbital eccentricity
 * Q_factor: tidal dissipation factor (Earth: ~100, rocky: 10-500)
 * rigidity: rigidity modulus Pa (Earth mantle: ~1e11)
 * stellar_age_s: age of system
 * orbital_period_s: for synchronous period
 */
tidal_t tidal_compute(double planet_mass_kg, double planet_radius_m,
                      double semi_major_m, double stellar_mass_kg,
                      double eccentricity, double Q_factor,
                      double rigidity, double stellar_age_s,
                      double orbital_period_s);

#endif /* TIDAL_H */
