#include <math.h>
#include "planck.h"
#include "../../data/constants.h"

/* Wien's displacement constant b = hc/k * (1/4.965...) */
#define WIEN_B  2.897771955e-3   /* m*K */

/* UV/VIS/IR boundary wavelengths, meters */
#define LAMBDA_UV_MAX   400e-9
#define LAMBDA_VIS_MAX  700e-9
#define LAMBDA_IR_MIN   700e-9
#define LAMBDA_IR_MAX   1e-3

#define PI  3.14159265358979323846

double planck_spectral_radiance(double lambda, double T)
{
    double c1 = 2.0 * PLANCK_CONST * SPEED_OF_LIGHT * SPEED_OF_LIGHT;
    double c2 = (PLANCK_CONST * SPEED_OF_LIGHT) / (BOLTZMANN * T);

    /*
     * B_lambda = (2hc^2 / lambda^5) / (exp(hc/lambda*kT) - 1)
     * Guard against overflow in exp for very short wavelengths.
     */
    double exponent = c2 / lambda;
    if (exponent > 700.0)
        return 0.0;

    return (c1 / pow(lambda, 5.0)) / (exp(exponent) - 1.0);
}

double planck_peak_wavelength(double T)
{
    return WIEN_B / T;
}

double planck_total_power(double T)
{
    return SIGMA * T * T * T * T;
}

/*
 * Band-integrated spectral radiance using composite Simpson's rule.
 * Integrand is B_lambda * pi (hemispherical emittance conversion).
 * n_steps must be even.
 */
double planck_band_flux(double T, double lmin, double lmax, int n_steps)
{
    if (n_steps % 2 != 0)
        n_steps++;

    double h   = (lmax - lmin) / n_steps;
    double sum = planck_spectral_radiance(lmin, T)
               + planck_spectral_radiance(lmax, T);

    for (int i = 1; i < n_steps; i++) {
        double lambda = lmin + i * h;
        double weight = (i % 2 == 0) ? 2.0 : 4.0;
        sum += weight * planck_spectral_radiance(lambda, T);
    }

    return PI * (h / 3.0) * sum;
}

spectral_t planck_analyze(double T)
{
    spectral_t sp;

    sp.stellar_color_temp  = T;
    sp.peak_wavelength_nm  = planck_peak_wavelength(T) * 1e9;
    sp.total_power         = planck_total_power(T);

    /*
     * Band integration with 1000 steps per band.
     * UV lower bound: 10 nm (soft x-ray cutoff for stellar atmosphere).
     */
    double uv_flux  = planck_band_flux(T, 10e-9,       LAMBDA_UV_MAX,  1000);
    double vis_flux = planck_band_flux(T, LAMBDA_UV_MAX, LAMBDA_VIS_MAX, 1000);
    double ir_flux  = planck_band_flux(T, LAMBDA_IR_MIN, LAMBDA_IR_MAX,  2000);

    sp.uv_fraction      = uv_flux  / sp.total_power;
    sp.visible_fraction = vis_flux / sp.total_power;
    sp.ir_fraction      = ir_flux  / sp.total_power;

    /* Classify dominant emission band */
    sp.peak_wavelength_band = sp.peak_wavelength_nm;

    return sp;
}
