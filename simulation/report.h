#ifndef REPORT_H
#define REPORT_H

#include "orbit.h"

void report_print_full(const orbit_solution_t *sol);
void report_print_orbital(const orbit_solution_t *sol);
void report_print_energy(const orbit_solution_t *sol);
void report_print_thermal(const orbit_solution_t *sol);
void report_print_tidal(const orbit_solution_t *sol);
void report_print_habitability(const orbit_solution_t *sol);
void report_print_stellar(const orbit_solution_t *sol);
void report_print_spectrum(const orbit_solution_t *sol);
void report_print_validation(const orbit_solution_t *sol);

/* Orbit state at specific mean anomaly (degrees) */
void report_print_orbit_state(const orbit_solution_t *sol, double mean_anomaly_deg);

#endif /* REPORT_H */
