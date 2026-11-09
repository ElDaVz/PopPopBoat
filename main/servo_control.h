#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { SERVO_MIN_TEST_MS = 20, SERVO_MAX_TEST_MS = 5000 };

typedef struct {
    uint16_t stop_us;
    uint16_t to_zero_ms;
    uint16_t to_180_ms;
    bool calibrated;
} servo_config_t;

typedef struct {
    int angle;
    int target;
    bool referenced;
    bool moving;
    bool testing;
    uint16_t pulse_us;
    uint32_t remaining_ms;
} servo_status_t;

servo_config_t servo_control_defaults(void);
bool servo_control_valid_config(const servo_config_t *config);
void servo_control_init(const servo_config_t *config);
bool servo_control_configure(const servo_config_t *config, uint32_t now);
bool servo_control_reference(void);
bool servo_control_target(int angle, uint32_t now);
bool servo_control_test(int direction, uint32_t duration_ms, uint32_t now);
void servo_control_stop(uint32_t now);
void servo_control_tick(uint32_t now);
servo_status_t servo_control_status(void);
servo_config_t servo_control_config(void);

#ifdef __cplusplus
}
#endif
