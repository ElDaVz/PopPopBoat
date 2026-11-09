#include "servo_control.h"

/* Servo continuo: posicion estimada por tiempo, nunca medida. */
static servo_config_t config;
static servo_status_t state;
static int32_t position_mdeg;
static int32_t start_mdeg;
static int32_t destination_mdeg;
static uint32_t started_at;
static uint32_t duration;

servo_config_t servo_control_defaults(void)
{
    /* Tiempos iniciales para probar, NO son una calibracion de 90 grados. */
    return (servo_config_t){1500, 200, 200, false};
}

bool servo_control_valid_config(const servo_config_t *candidate)
{
    return candidate && candidate->stop_us >= 1300 && candidate->stop_us <= 1700 &&
           candidate->to_zero_ms >= SERVO_MIN_TEST_MS &&
           candidate->to_zero_ms <= SERVO_MAX_TEST_MS &&
           candidate->to_180_ms >= SERVO_MIN_TEST_MS &&
           candidate->to_180_ms <= SERVO_MAX_TEST_MS;
}

void servo_control_init(const servo_config_t *candidate)
{
    config = servo_control_valid_config(candidate) ? *candidate : servo_control_defaults();
    state = (servo_status_t){.angle = 90, .target = 90, .pulse_us = config.stop_us};
    position_mdeg = start_mdeg = destination_mdeg = 90000;
    started_at = duration = 0;
    /* Nunca restaurar una referencia de posicion despues de un reinicio. */
}

void servo_control_tick(uint32_t now)
{
    if (!state.moving) return;
    const uint32_t elapsed = (uint32_t)(now - started_at);
    const uint32_t used = elapsed < duration ? elapsed : duration;
    if (!state.testing) {
        position_mdeg = start_mdeg +
            (int32_t)(((int64_t)(destination_mdeg - start_mdeg) * used) / duration);
        state.angle = (int)((position_mdeg + 500) / 1000);
    }
    state.remaining_ms = duration - used;
    if (elapsed >= duration) {
        state.moving = false;
        state.testing = false;
        state.pulse_us = config.stop_us;
    }
}

void servo_control_stop(uint32_t now)
{
    servo_control_tick(now);
    state.moving = false;
    state.testing = false;
    state.remaining_ms = 0;
    state.pulse_us = config.stop_us;
    state.target = state.angle;
}

bool servo_control_configure(const servo_config_t *candidate, uint32_t now)
{
    servo_control_tick(now);
    if (state.moving || !servo_control_valid_config(candidate)) return false;
    config = *candidate;
    state.referenced = false;
    state.pulse_us = config.stop_us;
    return true;
}

bool servo_control_reference(void)
{
    if (state.moving || !config.calibrated) return false;
    position_mdeg = 90000;
    state.angle = state.target = 90;
    state.referenced = true;
    return true;
}

bool servo_control_target(int angle, uint32_t now)
{
    servo_control_tick(now);
    if (!config.calibrated || !state.referenced || state.moving ||
        (angle != 0 && angle != 90 && angle != 180)) return false;
    state.target = angle;
    const int32_t delta = angle * 1000 - position_mdeg;
    if (delta == 0) return true;
    const uint32_t distance = (uint32_t)(delta < 0 ? -delta : delta);
    const uint32_t quarter_ms = delta < 0 ? config.to_zero_ms : config.to_180_ms;
    duration = (distance * quarter_ms + 89999) / 90000;
    started_at = now;
    start_mdeg = position_mdeg;
    destination_mdeg = angle * 1000;
    state.moving = true;
    state.testing = false;
    state.remaining_ms = duration;
    state.pulse_us = config.stop_us + (delta < 0 ? -100 : 100);
    return true;
}

bool servo_control_test(int direction, uint32_t duration_ms, uint32_t now)
{
    servo_control_tick(now);
    if (state.moving || direction < -1 || direction > 1 ||
        duration_ms < SERVO_MIN_TEST_MS || duration_ms > SERVO_MAX_TEST_MS) return false;
    state.referenced = false;
    state.moving = state.testing = true;
    state.remaining_ms = duration_ms;
    state.pulse_us = config.stop_us + direction * 100;
    started_at = now;
    duration = duration_ms;
    return true;
}

servo_status_t servo_control_status(void) { return state; }
servo_config_t servo_control_config(void) { return config; }
