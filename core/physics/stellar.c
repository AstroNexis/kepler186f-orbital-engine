#include <math.h>
#include <string.h>
#include <stdio.h>
#include "stellar.h"
#include "../../data/constants.h"

/*
 * Main sequence lifetime proxy:
 *   t_ms ~ (M/M_sun) / (L/L_sun) * t_sun
 * where t_sun = 10 Gyr.
 * Valid to order of magnitude for M_star in [0.1, 100] M_sun.
 */
static double main_seq_lifetime(double mass_kg, double luminosity_w)
{
    double mass_rel = mass_kg / M_SUN;
    double lum_rel  = luminosity_w / L_SUN;
    return (mass_rel / lum_rel) * 1e10;   /* years */
}

/*
 * Surface gravity: g = G*M / R^2
 * log g in cgs: M in grams, R in cm — multiply SI by 100 for cgs.
 */
static double surface_gravity(double mass_kg, double radius_m)
{
    return G * mass_kg / (radius_m * radius_m);
}

static double log_g_cgs(double g_si)
{
    return log10(g_si * 100.0);
}

static double escape_velocity(double mass_kg, double radius_m)
{
    return sqrt(2.0 * G * mass_kg / radius_m);
}

static spectral_class_t classify_spectral(double teff)
{
    if (teff >= 30000.0) return SPECTRAL_O;
    if (teff >= 10000.0) return SPECTRAL_B;
    if (teff >=  7500.0) return SPECTRAL_A;
    if (teff >=  6000.0) return SPECTRAL_F;
    if (teff >=  5200.0) return SPECTRAL_G;
    if (teff >=  3700.0) return SPECTRAL_K;
    if (teff >=  2400.0) return SPECTRAL_M;
    return SPECTRAL_UNKNOWN;
}

const char *spectral_class_name(spectral_class_t cls)
{
    static const char *names[] = { "O", "B", "A", "F", "G", "K", "M", "?" };
    return names[(int)cls];
}

stellar_t stellar_compute(double mass_kg, double radius_m,
                           double luminosity_w, double teff_k)
{
    stellar_t st;

    st.mass       = mass_kg;
    st.radius     = radius_m;
    st.luminosity = luminosity_w;
    st.teff       = teff_k;

    st.mass_rel      = mass_kg    / M_SUN;
    st.radius_rel    = radius_m   / R_SUN;
    st.luminosity_rel = luminosity_w / L_SUN;

    st.surface_gravity     = surface_gravity(mass_kg, radius_m);
    st.log_surface_gravity = log_g_cgs(st.surface_gravity);
    st.escape_velocity     = escape_velocity(mass_kg, radius_m);

    st.main_seq_lifetime_yr  = main_seq_lifetime(mass_kg, luminosity_w);
    st.main_seq_lifetime_gyr = st.main_seq_lifetime_yr / 1e9;

    st.spectral_class = classify_spectral(teff_k);
    snprintf(st.spectral_label, sizeof(st.spectral_label), "%s",
             spectral_class_name(st.spectral_class));

    return st;
}
