#ifndef PHYSICS_PLANCK_H
#define PHYSICS_PLANCK_H

/*
 * Planck spectral radiance and integrated quantities.
 * All wavelengths in meters, temperatures in Kelvin.
 */

/* B_lambda(T): spectral radiance, W sr^-1 m^-2 m^-1 */
double planck_spectral_radiance(double wavelength_m, double temperature_k);

/* Wien's displacement law: lambda_max = b/T */
double planck_peak_wavelength(double temperature_k);

/* Total power emitted per unit area: sigma*T^4 */
double planck_total_power(double temperature_k);

/*
 * Band-integrated flux between lambda_min and lambda_max.
 * Uses Simpson's rule with n_steps intervals.
 */
double planck_band_flux(double temperature_k,
                        double lambda_min, double lambda_max,
                        int n_steps);

typedef struct {
    double peak_wavelength_nm;      /* nm */
    double peak_wavelength_band;    /* UV / visible / IR classification */
    double total_power;             /* W/m^2 */
    double uv_fraction;             /* fraction of power in UV (<400nm) */
    double visible_fraction;        /* fraction in visible (400-700nm) */
    double ir_fraction;             /* fraction in IR (>700nm) */
    double stellar_color_temp;      /* K */
} spectral_t;

spectral_t planck_analyze(double temperature_k);

#endif /* PHYSICS_PLANCK_H */
