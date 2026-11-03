/*
 * Dandelion OS v4.1.1 - Fox Daemon Closed-Loop Thermal-Entropy Heuristic (src/fox_daemon_core.c)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 */
#include <stdio.h>
#include <stdint.h>

#define HOBO_RIG_POWER_CAP_W   18.5
#define THERMAL_SLOPE_MAX_CS   0.05
#define BASE_L1_THRESHOLD      0.042

typedef struct {
    double junction_temp_c;
    double delta_temp_cs;
    double power_draw_w;
    double entropy_ds;
    double active_l1_threshold;
} fox_telemetry_t;

int fox_daemon_step_feedback(fox_telemetry_t *state) {
    if (!state || state->entropy_ds < 0.0) return -1;
    if (state->delta_temp_cs > THERMAL_SLOPE_MAX_CS || state->power_draw_w > 16.5) {
        state->active_l1_threshold = BASE_L1_THRESHOLD * 1.25;
    } else {
        state->active_l1_threshold = BASE_L1_THRESHOLD;
    }
    return 0;
}
