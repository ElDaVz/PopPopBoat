#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "servo_control.h"
#include "web_page.h"

static constexpr uint8_t SERVO_PIN = D5; // NodeMCU D5 = GPIO14
static constexpr uint32_t CONFIG_MAGIC = 0x50424301;
struct StoredConfig {
    uint32_t magic;
    uint16_t stop_us, to_zero_ms, to_180_ms, checksum;
};
static Servo rudder;
static ESP8266WebServer server(80);
static bool server_ready = false;
static uint16_t last_pulse = 0;
static uint32_t last_report = 0;

static uint16_t checksum(const StoredConfig &value)
{
    return value.stop_us ^ value.to_zero_ms ^ value.to_180_ms ^ 0xa5a5;
}

static void apply_output()
{
    const servo_status_t state = servo_control_status();
    if (state.moving) {
        if (!rudder.attached()) {
            rudder.attach(SERVO_PIN, 1000, 2000, state.pulse_us);
            last_pulse = state.pulse_us;
        } else if (last_pulse != state.pulse_us) {
            rudder.writeMicroseconds(state.pulse_us);
            last_pulse = state.pulse_us;
        }
    } else if (rudder.attached()) {
        rudder.writeMicroseconds(state.pulse_us);
        rudder.detach();
        digitalWrite(SERVO_PIN, LOW);
        last_pulse = 0;
    }
}

static void send_status()
{
    servo_control_tick(millis());
    apply_output();
    const servo_status_t state = servo_control_status();
    const servo_config_t config = servo_control_config();
    char body[384];
    snprintf(body, sizeof(body),
        "{\"angle\":%d,\"target\":%d,\"referenced\":%s,\"moving\":%s,"
        "\"testing\":%s,\"calibrated\":%s,\"remaining_ms\":%lu,"
        "\"stop_us\":%u,\"to_zero_ms\":%u,\"to_180_ms\":%u,\"uptime_s\":%lu}",
        state.angle, state.target, state.referenced ? "true" : "false",
        state.moving ? "true" : "false", state.testing ? "true" : "false",
        config.calibrated ? "true" : "false", static_cast<unsigned long>(state.remaining_ms),
        static_cast<unsigned>(config.stop_us), static_cast<unsigned>(config.to_zero_ms),
        static_cast<unsigned>(config.to_180_ms), static_cast<unsigned long>(millis() / 1000));
    server.sendHeader("Cache-Control", "no-store");
    server.send(200, "application/json", body);
}

static void error(int code, const char *message)
{
    server.sendHeader("Cache-Control", "no-store");
    server.send(code, "text/plain; charset=utf-8", message);
}

static bool number(const char *key, int minimum, int maximum, int &value)
{
    if (!server.hasArg(key)) return false;
    const String text = server.arg(key);
    if (text.length() == 0 || text.length() > 4) return false;
    int parsed = 0;
    for (unsigned i = 0; i < text.length(); ++i) {
        if (text[i] < '0' || text[i] > '9') return false;
        parsed = parsed * 10 + text[i] - '0';
    }
    if (parsed < minimum || parsed > maximum) return false;
    value = parsed;
    return true;
}

static bool read_config(servo_config_t &config)
{
    int neutral, zero, end;
    if (!number("stop_us", 1300, 1700, neutral) ||
        !number("to_zero_ms", 20, 5000, zero) ||
        !number("to_180_ms", 20, 5000, end)) return false;
    config = {static_cast<uint16_t>(neutral), static_cast<uint16_t>(zero),
              static_cast<uint16_t>(end), false};
    return true;
}

static void register_routes()
{
    server.on("/", HTTP_GET, []() {
        server.sendHeader("Cache-Control", "no-store");
        server.send_P(200, "text/html; charset=utf-8", WEB_PAGE);
    });
    server.on("/api/status", HTTP_GET, send_status);
    server.on("/api/stop", HTTP_POST, []() {
        servo_control_stop(millis());
        apply_output();
        send_status();
    });
    server.on("/api/angle", HTTP_POST, []() {
        int value;
        if (!number("value", 0, 180, value) || (value != 0 && value != 90 && value != 180)) {
            error(400, "Elige 0, 90 o 180 grados");
        } else if (!servo_control_target(value, millis())) {
            error(409, "Primero calibra y marca el centro; espera a que termine el movimiento");
        } else {
            apply_output();
            send_status();
        }
    });
    server.on("/api/reference", HTTP_POST, []() {
        servo_control_tick(millis());
        if (!servo_control_reference()) error(409, "Calibra y detiene el servo antes de marcar el centro");
        else send_status();
    });
    server.on("/api/test", HTTP_POST, []() {
        servo_config_t config;
        int direction, duration;
        if (!read_config(config) || !number("direction", 0, 2, direction) ||
            !number("duration_ms", 20, 5000, duration)) {
            error(400, "Parametros de prueba invalidos");
            return;
        }
        // Una prueba con los mismos valores no borra la calibracion guardada.
        const servo_config_t previous = servo_control_config();
        config.calibrated = previous.calibrated && previous.stop_us == config.stop_us &&
            previous.to_zero_ms == config.to_zero_ms && previous.to_180_ms == config.to_180_ms;
        if (!servo_control_configure(&config, millis()) ||
            !servo_control_test(direction - 1, duration, millis())) {
            error(409, "Detiene el movimiento antes de probar");
            return;
        }
        apply_output();
        send_status();
    });
    server.on("/api/calibration", HTTP_POST, []() {
        servo_config_t config;
        if (!read_config(config) || server.arg("confirmed") != "1") {
            error(400, "Confirma que mediste 90 grados en ambos sentidos");
            return;
        }
        config.calibrated = true;
        if (servo_control_status().moving) {
            error(409, "Detiene el movimiento antes de guardar");
            return;
        }
        const StoredConfig stored = {CONFIG_MAGIC, config.stop_us, config.to_zero_ms,
            config.to_180_ms, static_cast<uint16_t>(config.stop_us ^ config.to_zero_ms ^ config.to_180_ms ^ 0xa5a5)};
        EEPROM.put(0, stored);
        if (!EEPROM.commit()) {
            error(500, "No se pudo guardar la calibracion");
            return;
        }
        servo_control_configure(&config, millis());
        send_status();
    });
    // Una pagina antigua no debe reactivar el barrido ni controlar velocidades.
    server.on("/api/auto", HTTP_ANY, []() {
        servo_control_stop(millis());
        apply_output();
        error(410, "Barrido retirado: recarga la pagina para el servo continuo");
    });
    server.onNotFound([]() { error(404, "Ruta no encontrada; recarga la pagina"); });
}

void setup()
{
    // Sin pulsos al encender: no asumir que 1500 us es el neutro exacto.
    pinMode(SERVO_PIN, OUTPUT);
    digitalWrite(SERVO_PIN, LOW);
    Serial.begin(115200);
    delay(200);
    EEPROM.begin(64);
    StoredConfig stored = {};
    EEPROM.get(0, stored);
    servo_config_t config = servo_control_defaults();
    if (stored.magic == CONFIG_MAGIC && stored.checksum == checksum(stored)) {
        const servo_config_t saved = {stored.stop_us, stored.to_zero_ms, stored.to_180_ms, true};
        if (servo_control_valid_config(&saved)) config = saved;
    }
    servo_control_init(&config);
    Serial.printf("\nPopPopBoat CONTINUO: sin movimiento; calibrado=%s; referencia pendiente\n",
                  config.calibrated ? "si" : "no");
    WiFi.persistent(false);
    if (!WiFi.mode(WIFI_AP) || !WiFi.softAP("PopPopBoat", "", 6, false, 4)) {
        Serial.println("ERROR WiFi; salida del servo desactivada");
        return;
    }
    register_routes();
    server.begin();
    server_ready = true;
    Serial.println("PopPopBoat: Servidor HTTP iniciado http://192.168.4.1; servo D5/GPIO14");
}

void loop()
{
    servo_control_tick(millis());
    apply_output();
    if (server_ready) server.handleClient();
    servo_control_tick(millis());
    apply_output();
    const uint32_t now = millis();
    if (now - last_report >= 10000) {
        last_report = now;
        const servo_status_t state = servo_control_status();
        Serial.printf("PopPopBoat: uptime=%lu s; movimiento=%s; referencia=%s; heap=%u\n",
            static_cast<unsigned long>(now / 1000), state.moving ? "si" : "no",
            state.referenced ? "si" : "no", ESP.getFreeHeap());
    }
    delay(1);
}
