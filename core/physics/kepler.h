#ifndef KEPLER_H
#define KEPLER_H

#define KEPLER_NR_MAX_ITER  100
#define KEPLER_NR_TOL       1e-12

typedef struct {
    double period;              /* s */
    double stellar_mass;        /* kg */
    double eccentricity;
    double semi_major_axis;     /* m, derived */
    double semi_minor_axis;     /* m */
    double periapsis;           /* m */
    double apoapsis;            /* m */
    double mean_motion;         /* rad/s */
    double orbital_velocity_circ; /* m/s, circular approximation at a */
    double angular_momentum;    /* m^2/s, specific */
} orbital_elements_t;

typedef struct {
    double mean_anomaly;        /* rad */
    double eccentric_anomaly;   /* rad, solved via Newton-Raphson */
    double true_anomaly;        /* rad */
    double radius;              /* m, instantaneous distance from focus */
    double velocity;            /* m/s, vis-viva */
    double velocity_radial;     /* m/s */
    double velocity_tangential; /* m/s */
    double flux_factor;         /* relative to flux at semi-major axis */
} orbital_state_t;

orbital_elements_t kepler_solve(double period_s, double stellar_mass_kg,
                                 double eccentricity);

/* Newton-Raphson solver for Kepler's equation: M = E - e*sin(E) */
double kepler_eccentric_anomaly(double mean_anomaly, double eccentricity);

double kepler_true_anomaly(double eccentric_anomaly, double eccentricity);

orbital_state_t kepler_state_at_anomaly(const orbital_elements_t *el,
                                         double mean_anomaly);

double kepler_verify(const orbital_elements_t *el);

#endif /* KEPLER_H */
