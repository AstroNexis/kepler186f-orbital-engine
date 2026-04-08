#ifndef HABITABILITY_H
#define HABITABILITY_H

/*
 * Habitable zone calculations using Kopparapu et al. 2013 (ApJ 765, 131).
 * Flux-based formulation with stellar T_eff corrections.
 * Valid for T_eff in [2600K, 7200K]; Kepler-186 (3788K) is within range.
 */

typedef enum {
    HZ_RECENT_VENUS    = 0,
    HZ_RUNAWAY_GH      = 1,
    HZ_MAX_GREENHOUSE  = 2,
    HZ_EARLY_MARS      = 3,
    HZ_LIMIT_COUNT     = 4
} hz_limit_t;

typedef struct {
    double s_eff[HZ_LIMIT_COUNT];       /* effective flux at each boundary */
    double distance_au[HZ_LIMIT_COUNT]; /* AU, orbital distance at each limit */
    double distance_m[HZ_LIMIT_COUNT];  /* meters */

    double planet_flux_rel;             /* planet's S_eff relative to Earth */
    double planet_distance_au;          /* AU */

    int    in_conservative_hz;          /* between RG and MG */
    int    in_optimistic_hz;            /* between RV and EM */

    double hz_center_au;                /* midpoint of conservative HZ */
    double hz_width_au;                 /* width of conservative HZ */
} habitability_t;

habitability_t habitability_compute(double stellar_luminosity,
                                     double stellar_teff,
                                     double planet_flux_si,
                                     double planet_distance_au);

#endif /* HABITABILITY_H */
