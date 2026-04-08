#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data/constants.h"
#include "simulation/orbit.h"
#include "simulation/report.h"

static void usage(const char *prog)
{
    fprintf(stderr, "Usage: %s [options]\n", prog);
    fprintf(stderr, "  --albedo     <0.0-1.0>   Bond albedo (default: %.3f)\n", K186F_ALBEDO);
    fprintf(stderr, "  --greenhouse <K>         Greenhouse forcing in K (default: %.1f)\n", K186F_GREENHOUSE_DT);
    fprintf(stderr, "  --emissivity <0.0-1.0>   Atmospheric emissivity (default: 0.78)\n");
    fprintf(stderr, "  --ecc        <0.0-0.99>  Orbital eccentricity (default: 0.0)\n");
    fprintf(stderr, "  --tidal-q    <value>     Tidal Q factor (default: 100)\n");
}

static double parse_double_arg(int argc, char **argv, const char *flag, double fallback)
{
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0) {
            return atof(argv[i + 1]);
        }
    }
    return fallback;
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    double albedo      = parse_double_arg(argc, argv, "--albedo",     K186F_ALBEDO);
    double greenhouse  = parse_double_arg(argc, argv, "--greenhouse", K186F_GREENHOUSE_DT);
    double emissivity  = parse_double_arg(argc, argv, "--emissivity", 0.78);
    double eccentricity = parse_double_arg(argc, argv, "--ecc",       K186F_ECCENTRICITY);
    double tidal_q     = parse_double_arg(argc, argv, "--tidal-q",    100.0);

    if (albedo < 0.0 || albedo > 1.0) {
        fprintf(stderr, "error: albedo must be in [0.0, 1.0]\n");
        return EXIT_FAILURE;
    }
    if (eccentricity < 0.0 || eccentricity >= 1.0) {
        fprintf(stderr, "error: eccentricity must be in [0.0, 1.0)\n");
        return EXIT_FAILURE;
    }
    if (emissivity < 0.0 || emissivity > 1.0) {
        fprintf(stderr, "error: emissivity must be in [0.0, 1.0]\n");
        return EXIT_FAILURE;
    }

    orbit_params_t p;
    p.period_s          = K186F_PERIOD;
    p.stellar_mass      = K186_MASS;
    p.stellar_radius    = K186_RADIUS;
    p.stellar_luminosity = K186_LUMINOSITY;
    p.stellar_teff      = K186_TEFF;
    p.stellar_age_s     = K186_AGE_GYR * 1e9 * YEAR_SECONDS;
    p.planet_mass       = K186F_MASS;
    p.planet_radius     = K186F_RADIUS;
    p.eccentricity      = eccentricity;
    p.albedo            = albedo;
    p.greenhouse_dt     = greenhouse;
    p.emissivity        = emissivity;
    p.tidal_Q           = tidal_q;
    p.tidal_rigidity    = 1e11;   /* Pa, Earth mantle proxy for rocky planet */

    orbit_solution_t sol = orbit_analyze(&p);
    report_print_full(&sol);

    return EXIT_SUCCESS;
}
