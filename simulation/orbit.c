#include "orbit.h"
#include "../data/constants.h"

orbit_solution_t orbit_analyze(const orbit_params_t *p)
{
    orbit_solution_t sol;

    sol.orbital = kepler_solve(p->period_s, p->stellar_mass, p->eccentricity);

    sol.energy = energy_compute(p->stellar_luminosity,
                                 sol.orbital.semi_major_axis,
                                 p->eccentricity,
                                 p->planet_radius,
                                 p->albedo);

    sol.thermal = temperature_compute(sol.energy.stellar_flux_si,
                                       p->albedo,
                                       p->greenhouse_dt,
                                       p->emissivity);

    sol.tidal = tidal_compute(p->planet_mass, p->planet_radius,
                               sol.orbital.semi_major_axis,
                               p->stellar_mass,
                               p->stellar_radius,
                               p->eccentricity,
                               p->tidal_Q,
                               p->tidal_rigidity,
                               p->stellar_age_s,
                               p->period_s);

    sol.habitability = habitability_compute(p->stellar_luminosity,
                                             p->stellar_teff,
                                             sol.energy.stellar_flux_si,
                                             sol.orbital.semi_major_axis / AU);

    sol.stellar = stellar_compute(p->stellar_mass, p->stellar_radius,
                                   p->stellar_luminosity, p->stellar_teff);

    sol.stellar_spectrum = planck_analyze(p->stellar_teff);

    sol.kepler_error = kepler_verify(&sol.orbital);

    return sol;
}
