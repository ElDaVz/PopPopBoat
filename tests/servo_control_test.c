#include <assert.h>
#include <stdio.h>
#include "servo_control.h"

int main(void)
{
    servo_config_t config = servo_control_defaults();
    servo_control_init(&config);
    assert(!servo_control_status().moving);
    assert(!servo_control_status().referenced);
    assert(!servo_control_target(0, 0));
    assert(!servo_control_reference());
    assert(!servo_control_test(-2, 100, 0));
    assert(!servo_control_test(1, 0, 0));
    assert(!servo_control_test(1, 5001, 0));
    assert(servo_control_test(1, 100, 0));
    assert(servo_control_status().pulse_us == 1600);
    servo_control_tick(99);
    assert(servo_control_status().moving);
    servo_control_tick(100);
    assert(!servo_control_status().moving);
    assert(servo_control_status().pulse_us == 1500);

    config.calibrated = true;
    config.to_zero_ms = 200;
    config.to_180_ms = 300;
    assert(servo_control_configure(&config, 100));
    assert(servo_control_reference());
    assert(!servo_control_target(45, 100));
    assert(servo_control_target(0, 100));
    assert(servo_control_status().remaining_ms == 200);
    assert(servo_control_status().pulse_us == 1400);
    assert(!servo_control_reference());
    assert(!servo_control_target(180, 150));
    assert(!servo_control_configure(&config, 150));
    servo_control_tick(200);
    assert(servo_control_status().angle == 45);
    servo_control_stop(200);
    assert(!servo_control_status().moving);
    assert(servo_control_status().target == 45);
    assert(servo_control_status().pulse_us == 1500);
    assert(servo_control_target(90, 200));
    assert(servo_control_status().remaining_ms == 150);
    servo_control_tick(350);
    assert(servo_control_status().angle == 90);
    assert(servo_control_target(180, 350));
    servo_control_tick(650);
    assert(servo_control_status().angle == 180);
    assert(servo_control_target(0, 650));
    assert(servo_control_status().remaining_ms == 400);
    servo_control_tick(1050);
    assert(servo_control_status().angle == 0);

    assert(servo_control_test(-1, 40, 1050));
    assert(!servo_control_status().referenced);
    servo_control_tick(1090);
    assert(!servo_control_target(90, 1090));
    servo_control_init(&config);
    assert(!servo_control_status().referenced); // reiniciar invalida el centro
    assert(servo_control_reference());
    assert(servo_control_target(0, UINT32_MAX - 99));
    servo_control_tick(99);
    assert(servo_control_status().moving);
    servo_control_tick(100);
    assert(!servo_control_status().moving);
    assert(servo_control_status().angle == 0);

    config.stop_us = 2000;
    assert(!servo_control_valid_config(&config));
    servo_control_init(&config);
    assert(!servo_control_config().calibrated);
    assert(servo_control_status().pulse_us == 1500);
    config = (servo_config_t){1700, 5000, 5000, true};
    assert(servo_control_configure(&config, 0));
    assert(servo_control_reference());
    assert(servo_control_target(180, 0));
    servo_control_tick(5000);
    assert(servo_control_status().angle == 180);
    assert(servo_control_target(0, 5000));
    assert(servo_control_status().remaining_ms == 10000);
    servo_control_tick(15000);
    assert(servo_control_status().angle == 0 && !servo_control_status().moving);
    puts("OK: sin giro inicial, referencia, limites, tiempos por sentido, parada, pruebas acotadas y rollover");
}
