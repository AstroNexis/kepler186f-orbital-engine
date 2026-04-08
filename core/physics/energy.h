#ifndef ENERGY_H
#define ENERGY_H

typedef struct {
    double stellar_flux_si;     /* W/m^2 at semi-major axis */
    double stellar_flux_rel;    /* normalized to Earth insolation */
    double flux_periapsis;      /* W/m^2 at closest approach */
    double flux_apoapsis;       /* W/m^2 at farthest point */
    double flux_variation;      /* (max-min)/mean, dimensionless */
    double power_absorbed;      /* W, total power absorbed by planet */
    double power_emitted;       /* W, blackbody re-emission */
} energy_t;

energy_t energy_compute(double stellar_luminosity, double semi_major_axis,
                        double eccentricity, double planet_radius,
                        double albedo);

double flux_at_radius(double luminosity, double radius);

#endif /* ENERGY_H */
