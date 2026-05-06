#include <math.h>
#include <stdio.h>
#include "kepler.h"
#include "../../data/constants.h"

#define PI  3.14159265358979323846

static double derive_semi_major_axis(double period_s, double stellar_mass_kg)
{
    double num = G * stellar_mass_kg * period_s * period_s;
    double den = 4.0 * PI * PI;
    return cbrt(num / den);
}

orbital_elements_t kepler_solve(double period_s, double stellar_mass_kg,
                                 double eccentricity)
{
    orbital_elements_t el;

    el.period        = period_s;
    el.stellar_mass  = stellar_mass_kg;
    el.eccentricity  = eccentricity;

    el.semi_major_axis = derive_semi_major_axis(period_s, stellar_mass_kg);

    /* b = a * sqrt(1 - e^2) */
    el.semi_minor_axis = el.semi_major_axis * sqrt(1.0 - eccentricity * eccentricity);

    el.periapsis = el.semi_major_axis * (1.0 - eccentricity);
    el.apoapsis  = el.semi_major_axis * (1.0 + eccentricity);

    el.mean_motion = (2.0 * PI) / period_s;

    /* circular velocity at a — useful reference even for e>0 */
    el.orbital_velocity_circ = (2.0 * PI * el.semi_major_axis) / period_s;

    /* h = sqrt(G * M * a * (1 - e^2)), specific angular momentum */
    el.angular_momentum = sqrt(G * stellar_mass_kg * el.semi_major_axis
                               * (1.0 - eccentricity * eccentricity));

    return el;
}

/*
 * Kepler's equation: M = E - e*sin(E)
 * Newton-Raphson iteration: E_{n+1} = E_n - f(E_n)/f'(E_n)
 * Converges quadratically; typically <10 iterations for e < 0.9.
 */
double kepler_eccentric_anomaly(double mean_anomaly, double eccentricity)
{
    double e = eccentricity;
    double M = mean_anomaly;

    /* Danby initial guess — better than E=M for moderate eccentricity */
    double E = M + e * sin(M) * (1.0 + e * cos(M));

    int converged = 0;
    for (int i = 0; i < KEPLER_NR_MAX_ITER; i++) {
        double dE = (M - E + e * sin(E)) / (1.0 - e * cos(E));
        E += dE;
        if (fabs(dE) < KEPLER_NR_TOL) {
            converged = 1;
            break;
        }
    }

    if (!converged) {
        /* Residual still above tolerance — return NaN so callers can detect. */
        fprintf(stderr,
                "kepler_eccentric_anomaly: NR did not converge "
                "(M=%.6f, e=%.6f, residual=%.2e)\n",
                M, e, fabs(M - E + e * sin(E)));
        return __builtin_nan("");
    }

    return E;
}

/*
 * True anomaly from eccentric anomaly.
 * Using atan2 preserves quadrant and avoids division by near-zero.
 */
double kepler_true_anomaly(double E, double eccentricity)
{
    double e = eccentricity;
    double sin_nu = (sqrt(1.0 - e * e) * sin(E)) / (1.0 - e * cos(E));
    double cos_nu = (cos(E) - e) / (1.0 - e * cos(E));
    return atan2(sin_nu, cos_nu);
}

orbital_state_t kepler_state_at_anomaly(const orbital_elements_t *el,
                                         double mean_anomaly)
{
    orbital_state_t st;
    double e = el->eccentricity;
    double a = el->semi_major_axis;
    double mu = G * el->stellar_mass;

    st.mean_anomaly      = mean_anomaly;
    st.eccentric_anomaly = kepler_eccentric_anomaly(mean_anomaly, e);
    st.true_anomaly      = kepler_true_anomaly(st.eccentric_anomaly, e);

    /* r = a(1 - e*cos(E)) */
    st.radius = a * (1.0 - e * cos(st.eccentric_anomaly));

    /* vis-viva: v^2 = mu * (2/r - 1/a) */
    st.velocity = sqrt(mu * (2.0 / st.radius - 1.0 / a));

    /* radial: v_r = (mu/h) * e*sin(nu) */
    st.velocity_radial     = (mu / el->angular_momentum) * e * sin(st.true_anomaly);

    /* tangential: v_t = (mu/h) * (1 + e*cos(nu)) */
    st.velocity_tangential = (mu / el->angular_momentum) * (1.0 + e * cos(st.true_anomaly));

    /*
     * Flux scales as 1/r^2; normalize to flux at semi-major axis.
     * flux_factor > 1 near periapsis, < 1 near apoapsis.
     */
    st.flux_factor = (a * a) / (st.radius * st.radius);

    return st;
}

double kepler_verify(const orbital_elements_t *el)
{
    double a3 = pow(el->semi_major_axis, 3.0);
    double t2 = el->period * el->period;
    double ratio   = t2 / a3;
    double expected = (4.0 * PI * PI) / (G * el->stellar_mass);
    return fabs(ratio - expected) / expected;
}
