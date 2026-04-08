#include <stdio.h>
#include <math.h>
#include "report.h"
#include "../data/constants.h"

#define SEP  "------------------------------------------------------------\n"
#define HDR  "============================================================\n"

static void section(const char *title)
{
    printf("\n[%s]\n", title);
}

void report_print_orbital(const orbit_solution_t *sol)
{
    const orbital_elements_t *el = &sol->orbital;

    section("Orbital Mechanics");
    printf("  Semi-major axis          : %.6e m\n",    el->semi_major_axis);
    printf("  Semi-major axis          : %.6f AU\n",   el->semi_major_axis / AU);
    printf("  Semi-minor axis          : %.6e m\n",    el->semi_minor_axis);
    printf("  Orbital period           : %.4f days\n", el->period / 86400.0);
    printf("  Eccentricity             : %.4f\n",      el->eccentricity);
    printf("  Periapsis                : %.6e m  (%.4f AU)\n",
           el->periapsis, el->periapsis / AU);
    printf("  Apoapsis                 : %.6e m  (%.4f AU)\n",
           el->apoapsis, el->apoapsis / AU);
    printf("  Mean motion              : %.6e rad/s\n", el->mean_motion);
    printf("  Orbital velocity (circ.) : %.4f m/s  (%.4f km/s)\n",
           el->orbital_velocity_circ, el->orbital_velocity_circ / 1000.0);
    printf("  Specific ang. momentum   : %.6e m^2/s\n", el->angular_momentum);
}

void report_print_energy(const orbit_solution_t *sol)
{
    const energy_t *en = &sol->energy;

    section("Energy Model");
    printf("  Stellar flux at a        : %.4f W/m^2\n",    en->stellar_flux_si);
    printf("  Stellar flux (S_earth)   : %.6f\n",          en->stellar_flux_rel);
    printf("  Flux at periapsis        : %.4f W/m^2\n",    en->flux_periapsis);
    printf("  Flux at apoapsis         : %.4f W/m^2\n",    en->flux_apoapsis);
    printf("  Flux variation           : %.4f (peak-to-peak/mean)\n", en->flux_variation);
    printf("  Power absorbed           : %.4e W\n",        en->power_absorbed);
}

void report_print_thermal(const orbit_solution_t *sol)
{
    const thermal_t *th = &sol->thermal;

    section("Thermal / Climate");
    printf("  Bond albedo              : %.3f\n",  th->albedo);
    printf("  Atmospheric emissivity   : %.3f\n",  th->emissivity);
    printf("  Equilibrium temp (T_eq)  : %.2f K  (%.2f C)\n",
           th->equilibrium_temp, th->equilibrium_temp - 273.15);
    printf("  Surface temp (GH proxy)  : %.2f K  (%.2f C)\n",
           th->surface_temp_estimate, th->surface_temp_estimate - 273.15);
    printf("  Greenhouse forcing       : %.1f K\n", th->greenhouse_forcing);
    printf("  1-layer atm T_surface    : %.2f K  (%.2f C)\n",
           th->t_surface_1layer, th->t_surface_1layer - 273.15);
    printf("  Substellar point temp    : %.2f K  (%.2f C)\n",
           th->t_substellar, th->t_substellar - 273.15);
    printf("  Terminator temp          : %.2f K  (%.2f C)\n",
           th->t_terminator, th->t_terminator - 273.15);
    printf("  Anti-stellar point temp  : ~0 K    (no redistribution)\n");
}

void report_print_tidal(const orbit_solution_t *sol)
{
    const tidal_t *td = &sol->tidal;

    section("Tidal Mechanics");
    printf("  Tidal lock timescale     : %.4e yr  (%.4f Gyr)\n",
           td->tidal_lock_timescale_yr, td->tidal_lock_timescale_gyr);
    printf("  Likely tidally locked    : %s\n",
           td->is_likely_locked ? "YES" : "NO");
    printf("  Tidal heating power      : %.4e W\n",  td->tidal_heating_power);
    printf("  Tidal heating flux       : %.4e W/m^2\n", td->tidal_heating_flux);
    printf("  Sync rotation period     : %.4f days\n",
           td->sync_rotation_period_s / 86400.0);
    printf("  Roche limit              : %.4e m  (%.4f AU)\n",
           td->roche_limit, td->roche_limit / AU);
}

void report_print_habitability(const orbit_solution_t *sol)
{
    const habitability_t *hz = &sol->habitability;
    static const char *labels[HZ_LIMIT_COUNT] = {
        "Recent Venus (inner opt.)",
        "Runaway Greenhouse (inner)",
        "Max Greenhouse (outer)   ",
        "Early Mars (outer opt.)  "
    };

    section("Habitable Zone (Kopparapu 2013)");
    for (int i = 0; i < HZ_LIMIT_COUNT; i++) {
        printf("  %s : %.4f AU  (S_eff=%.4f)\n",
               labels[i], hz->distance_au[i], hz->s_eff[i]);
    }
    printf("\n");
    printf("  Planet distance          : %.4f AU\n",  hz->planet_distance_au);
    printf("  Planet S_eff             : %.4f S_earth\n", hz->planet_flux_rel);
    printf("  Conservative HZ center   : %.4f AU\n",  hz->hz_center_au);
    printf("  Conservative HZ width    : %.4f AU\n",  hz->hz_width_au);
    printf("  In conservative HZ       : %s\n",
           hz->in_conservative_hz ? "YES" : "NO");
    printf("  In optimistic HZ         : %s\n",
           hz->in_optimistic_hz   ? "YES" : "NO");
}

void report_print_stellar(const orbit_solution_t *sol)
{
    const stellar_t *st = &sol->stellar;

    section("Stellar Properties");
    printf("  Spectral class           : %s\n",     st->spectral_label);
    printf("  Mass                     : %.4f M_sun\n", st->mass_rel);
    printf("  Radius                   : %.4f R_sun\n", st->radius_rel);
    printf("  Luminosity               : %.4f L_sun  (%.4e W)\n",
           st->luminosity_rel, st->luminosity);
    printf("  T_eff                    : %.0f K\n",  st->teff);
    printf("  Surface gravity          : %.4f m/s^2\n", st->surface_gravity);
    printf("  log g (cgs)              : %.4f\n",    st->log_surface_gravity);
    printf("  Escape velocity          : %.4f km/s\n", st->escape_velocity / 1000.0);
    printf("  MS lifetime estimate     : %.4f Gyr\n", st->main_seq_lifetime_gyr);
}

void report_print_spectrum(const orbit_solution_t *sol)
{
    const spectral_t *sp = &sol->stellar_spectrum;

    section("Stellar Spectral Analysis (Planck)");
    printf("  Stellar T_eff            : %.0f K\n",  sp->stellar_color_temp);
    printf("  Peak emission wavelength : %.2f nm\n", sp->peak_wavelength_nm);
    printf("  Total power density      : %.4e W/m^2\n", sp->total_power);
    printf("  UV fraction  (<400nm)    : %.4f  (%.2f%%)\n",
           sp->uv_fraction,      sp->uv_fraction      * 100.0);
    printf("  VIS fraction (400-700nm) : %.4f  (%.2f%%)\n",
           sp->visible_fraction, sp->visible_fraction * 100.0);
    printf("  IR fraction  (>700nm)    : %.4f  (%.2f%%)\n",
           sp->ir_fraction,      sp->ir_fraction      * 100.0);
}

void report_print_validation(const orbit_solution_t *sol)
{
    section("Validation");
    printf("  Kepler T^2/a^3 residual  : %.4e (relative)\n", sol->kepler_error);
    printf("  Kepler consistency       : %s\n",
           sol->kepler_error < 1e-9 ? "PASS" : "FAIL");
}

void report_print_orbit_state(const orbit_solution_t *sol, double deg)
{
    double rad = deg * 3.14159265358979323846 / 180.0;
    orbital_state_t st = kepler_state_at_anomaly(&sol->orbital, rad);

    printf("\n  Orbit state at M = %.1f deg:\n", deg);
    printf("    Eccentric anomaly      : %.6f rad\n", st.eccentric_anomaly);
    printf("    True anomaly           : %.6f rad  (%.4f deg)\n",
           st.true_anomaly, st.true_anomaly * 180.0 / 3.14159265358979323846);
    printf("    Orbital radius         : %.6e m  (%.4f AU)\n",
           st.radius, st.radius / AU);
    printf("    Velocity (vis-viva)    : %.4f m/s  (%.4f km/s)\n",
           st.velocity, st.velocity / 1000.0);
    printf("    Radial velocity        : %.4f m/s\n", st.velocity_radial);
    printf("    Tangential velocity    : %.4f m/s\n", st.velocity_tangential);
    printf("    Flux factor            : %.6f\n", st.flux_factor);
}

void report_print_full(const orbit_solution_t *sol)
{
    printf(HDR);
    printf("  Kepler-186f Orbital Analysis Engine\n");
    printf(HDR);

    report_print_stellar(sol);
    report_print_spectrum(sol);
    report_print_orbital(sol);
    report_print_energy(sol);
    report_print_thermal(sol);
    report_print_tidal(sol);
    report_print_habitability(sol);

    /* Sample orbital states at cardinal mean anomalies */
    section("Orbital State Samples");
    report_print_orbit_state(sol, 0.0);
    report_print_orbit_state(sol, 90.0);
    report_print_orbit_state(sol, 180.0);
    report_print_orbit_state(sol, 270.0);

    report_print_validation(sol);

    printf("\n");
    printf(HDR);
}
