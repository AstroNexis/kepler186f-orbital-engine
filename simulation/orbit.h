#ifndef ORBIT_H
#define ORBIT_H

#include "../core/physics/kepler.h"
#include "../core/physics/energy.h"
#include "../core/physics/temperature.h"
#include "../core/physics/planck.h"
#include "../core/physics/tidal.h"
#include "../core/physics/habitability.h"
#include "../core/physics/stellar.h"
#include "../core/physics/planck.h"

typedef struct {
    orbital_elements_t orbital;
    energy_t           energy;
    thermal_t          thermal;
    tidal_t            tidal;
    habitability_t     habitability;
    stellar_t          stellar;
    spectral_t         stellar_spectrum;
    double             kepler_error;
} orbit_solution_t;

typedef struct {
    double period_s;
    double stellar_mass;
    double stellar_radius;
    double stellar_luminosity;
    double stellar_teff;
    double stellar_age_s;
    double planet_mass;
    double planet_radius;
    double eccentricity;
    double albedo;
    double greenhouse_dt;
    double emissivity;
    double tidal_Q;
    double tidal_rigidity;
} orbit_params_t;

orbit_solution_t orbit_analyze(const orbit_params_t *p);

#endif /* ORBIT_H */
