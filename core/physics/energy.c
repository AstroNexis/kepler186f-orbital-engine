#include <math.h>
#include "energy.h"
#include "../../data/constants.h"

#define PI  3.14159265358979323846

double flux_at_radius(double luminosity, double radius)
{
    return luminosity / (4.0 * PI * radius * radius);
}

energy_t energy_compute(double stellar_luminosity, double semi_major_axis,
                        double eccentricity, double planet_radius,
                        double albedo)
{
    energy_t en;

    double a  = semi_major_axis;
    double e  = eccentricity;
    double rp = a * (1.0 - e);    /* periapsis */
    double ra = a * (1.0 + e);    /* apoapsis */

    en.stellar_flux_si  = flux_at_radius(stellar_luminosity, a);
    en.stellar_flux_rel = en.stellar_flux_si / EARTH_INSOLATION;
    en.flux_periapsis   = flux_at_radius(stellar_luminosity, rp);
    en.flux_apoapsis    = flux_at_radius(stellar_luminosity, ra);

    /*
     * Flux variation: peak-to-peak normalized to orbit-averaged mean flux.
     * Mean flux over one orbit: <F> = L / (4*pi*a^2*sqrt(1-e^2))
     * Not the same as flux at a when e > 0.
     * For e=0 the formula evaluates to 0.0 naturally; no branch needed.
     */
    double flux_mean = flux_at_radius(stellar_luminosity, a) / sqrt(1.0 - e * e);
    en.flux_variation = (en.flux_periapsis - en.flux_apoapsis) / flux_mean;

    /* Cross-section pi*R^2 intercepts flux; (1-A) fraction absorbed */
    double cross_section = PI * planet_radius * planet_radius;
    en.power_absorbed    = en.stellar_flux_si * cross_section * (1.0 - albedo);

    /* Blackbody re-emission from full surface 4*pi*R^2 */
    en.power_emitted = en.power_absorbed;   /* steady-state */

    return en;
}
