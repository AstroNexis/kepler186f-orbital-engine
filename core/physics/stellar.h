#ifndef STELLAR_H
#define STELLAR_H

/*
 * Stellar physics module.
 * Main sequence lifetime, luminosity-mass relation, surface gravity,
 * escape velocity, and stellar classification.
 */

typedef enum {
    SPECTRAL_O = 0,
    SPECTRAL_B,
    SPECTRAL_A,
    SPECTRAL_F,
    SPECTRAL_G,
    SPECTRAL_K,
    SPECTRAL_M,
    SPECTRAL_UNKNOWN
} spectral_class_t;

typedef struct {
    double mass;                /* kg */
    double radius;              /* m */
    double luminosity;          /* W */
    double teff;                /* K */
    double surface_gravity;     /* m/s^2 */
    double log_surface_gravity; /* log10(g), cgs units */
    double escape_velocity;     /* m/s */
    double main_seq_lifetime_yr;/* years on main sequence */
    double main_seq_lifetime_gyr;
    double luminosity_rel;      /* L/L_sun */
    double mass_rel;            /* M/M_sun */
    double radius_rel;          /* R/R_sun */
    spectral_class_t spectral_class;
    char   spectral_label[4];
} stellar_t;

stellar_t stellar_compute(double mass_kg, double radius_m,
                           double luminosity_w, double teff_k);

const char *spectral_class_name(spectral_class_t cls);

#endif /* STELLAR_H */
