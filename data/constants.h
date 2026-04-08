#ifndef CONSTANTS_H
#define CONSTANTS_H

/* CODATA 2018 values unless noted */
#define G                   6.67430e-11       /* gravitational constant, m^3 kg^-1 s^-2 */
#define SIGMA               5.67037441e-8     /* Stefan-Boltzmann, W m^-2 K^-4 */
#define BOLTZMANN           1.380649e-23      /* Boltzmann constant, J K^-1 */
#define PLANCK_CONST        6.62607015e-34    /* Planck constant, J s */
#define SPEED_OF_LIGHT      2.99792458e8      /* m/s */
#define AU                  1.495978707e11    /* IAU 2012, meters */
#define PARSEC              3.085677581e16    /* meters */
#define YEAR_SECONDS        3.15576e7         /* Julian year */

/* Solar reference values (IAU 2015) */
#define M_SUN               1.989e30          /* kg */
#define R_SUN               6.957e8           /* meters */
#define L_SUN               3.828e26          /* W */
#define T_SUN               5778.0            /* K, effective temperature */

/* Earth reference values */
#define M_EARTH             5.972e24          /* kg */
#define R_EARTH             6.371e6           /* meters */
#define EARTH_INSOLATION    1361.0            /* W/m^2, solar constant IAU 2015 */
#define EARTH_ALBEDO        0.306
#define EARTH_SEMI_MAJOR    1.000             /* AU */

/*
 * Kepler-186 stellar parameters
 * Source: Quintana et al. 2014 (Science 344, 277)
 * Stellar type: M1V
 */
#define K186_MASS           (0.478  * M_SUN)
#define K186_RADIUS         (0.472  * R_SUN)
#define K186_LUMINOSITY     (0.0405 * L_SUN)
#define K186_TEFF           3788.0            /* K */
#define K186_AGE_GYR        4.0               /* Gyr, poorly constrained; use solar proxy */
#define K186_METALLICITY    0.0               /* [Fe/H], assume solar */

/*
 * Kepler-186f orbital and physical parameters
 * Period from transit photometry (Quintana et al. 2014).
 * Eccentricity unconstrained; assume e=0 (near-circular).
 * Mass unconstrained; assume 1.5 M_earth (rocky planet proxy).
 * Radius from transit depth: ~1.17 R_earth.
 */
#define K186F_PERIOD_DAYS   129.9444
#define K186F_PERIOD        (K186F_PERIOD_DAYS * 86400.0)
#define K186F_ECCENTRICITY  0.0
#define K186F_MASS          (1.5 * M_EARTH)
#define K186F_RADIUS        (1.17 * R_EARTH)

/*
 * Bond albedo: no direct measurement.
 * 0.306 is Earth proxy. Tunable via CLI --albedo.
 */
#define K186F_ALBEDO        0.306

/*
 * Greenhouse forcing delta: no atmosphere data.
 * Earth value is ~33 K. Used as upper-bound proxy.
 * Set to 0.0 to get pure blackbody equilibrium temperature.
 */
#define K186F_GREENHOUSE_DT 33.0

/* Kopparapu 2013 HZ coefficients — M-dwarf applicability range */
#define HZ_SEFF_INNER_RVL   1.0140            /* Recent Venus limit */
#define HZ_SEFF_INNER_RG    1.0512            /* Runaway Greenhouse */
#define HZ_SEFF_OUTER_MG    0.3438            /* Maximum Greenhouse */
#define HZ_SEFF_OUTER_EO    0.3179            /* Early Mars outer limit */

#endif /* CONSTANTS_H */
