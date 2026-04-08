#ifndef TEMPERATURE_H
#define TEMPERATURE_H

typedef struct {
    double albedo;
    double equilibrium_temp;        /* K, blackbody, no greenhouse */
    double surface_temp_estimate;   /* K, with simple greenhouse delta */
    double greenhouse_forcing;      /* K */

    /* 1-layer atmosphere model */
    double t_effective_1layer;      /* K, with single absorbing layer */
    double t_surface_1layer;        /* K */
    double emissivity;              /* assumed atmospheric emissivity */

    /* Diurnal range proxy (tidally locked estimate) */
    double t_substellar;            /* K, point directly under star */
    double t_antistellar;           /* K, night side (no redistribution) */
    double t_terminator;            /* K, limb approximation */
} thermal_t;

thermal_t temperature_compute(double stellar_flux_si, double albedo,
                               double greenhouse_dt, double emissivity);

#endif /* TEMPERATURE_H */
