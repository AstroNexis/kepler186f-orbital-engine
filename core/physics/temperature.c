#include <math.h>
#include "temperature.h"
#include "../../data/constants.h"

/*
 * Standard equilibrium temperature:
 *   T_eq = [F(1-A) / 4*sigma]^(1/4)
 * Factor of 4: ratio of absorbing cross-section to emitting surface.
 * No greenhouse, no heat redistribution asymmetry.
 */
static double t_equilibrium(double flux, double albedo)
{
    return pow((flux * (1.0 - albedo)) / (4.0 * SIGMA), 0.25);
}

/*
 * 1-layer atmosphere model (Sagan & Chyba style):
 * The atmosphere absorbs fraction (emissivity) of outgoing IR
 * and re-emits half upward, half downward.
 *
 *   T_surface^4 = T_eq^4 * (1 + emissivity/2) / (1 - emissivity/2)
 *
 * This is a linear approximation valid for small emissivity.
 * For emissivity = 1 (blackbody atmosphere) T_surface -> 2^(1/4) * T_eq.
 */
static double t_surface_1layer(double t_eq, double emissivity)
{
    double factor = (1.0 + emissivity / 2.0) / (1.0 - emissivity / 2.0);
    return t_eq * pow(factor, 0.25);
}

/*
 * Substellar point (tidally locked, no redistribution):
 *   T_sub = [F(1-A)/sigma]^(1/4)
 * Drops the factor of 4 because the substellar point receives
 * full normal incidence and the redistribution factor is 1 (no winds).
 */
static double t_substellar(double flux, double albedo)
{
    return pow((flux * (1.0 - albedo)) / SIGMA, 0.25);
}

/*
 * Terminator temperature — geometric projection factor of 0.25
 * is applied at the limb where incidence angle -> 90 degrees.
 * Approximation using cosine-averaged flux over a hemisphere quarter.
 */
static double t_terminator(double flux, double albedo)
{
    return pow((flux * (1.0 - albedo) * 0.25) / SIGMA, 0.25);
}

thermal_t temperature_compute(double stellar_flux_si, double albedo,
                               double greenhouse_dt, double emissivity)
{
    thermal_t th;

    th.albedo              = albedo;
    th.greenhouse_forcing  = greenhouse_dt;
    th.emissivity          = emissivity;

    th.equilibrium_temp        = t_equilibrium(stellar_flux_si, albedo);
    th.surface_temp_estimate   = th.equilibrium_temp + greenhouse_dt;
    th.t_effective_1layer      = t_equilibrium(stellar_flux_si, albedo);
    th.t_surface_1layer        = t_surface_1layer(th.t_effective_1layer, emissivity);

    th.t_substellar  = t_substellar(stellar_flux_si, albedo);
    /* Night side with no heat redistribution and no internal source.
     * Physical floor is the CMB (~2.7 K), not absolute zero. */
    th.t_antistellar = 2.7;
    th.t_terminator  = t_terminator(stellar_flux_si, albedo);

    return th;
}
