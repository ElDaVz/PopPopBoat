# PopPopBoat · NodeMCU ESP8266

Control local del timón desde el navegador del teléfono. El firmware crea la red
`PopPopBoat` (clave `<agregar>`) y sirve la interfaz en `http://192.168.4.1`.
No necesita internet, un servidor en la computadora ni mantener VS Code abierto.
Cada vez que recibe alimentación por micro-USB, la placa inicia el WiFi y el
servidor automáticamente. El teléfono debe permanecer conectado a `PopPopBoat`,
aunque iOS indique que esa red no tiene internet.

## Archivos y carga

- `main/main.c`: recorridos por tiempo, posición estimada y parada del servo continuo.
- `main/main.cpp`: WiFi, HTTP y PWM para ESP8266; salida **D5 / GPIO14**.
- `main/web_page.h`: interfaz sin dependencias externas, adaptable al teléfono.
- `platformio.ini`: NodeMCU 1.0, Arduino ESP8266 y herramientas fijadas a versión.
- `backups/esp32-before-esp8266.tar.gz`: versión ESP32 original y configuración.

Abrir **esta carpeta** en VS Code con PlatformIO. La extensión ESP-IDF y el
antiguo directorio `build/` corresponden a la placa ESP32; esta versión usa
`.pio/build/nodemcuv2`.

```sh
pio run
pio run --target upload --upload-port /dev/ttyUSB0
pio device monitor --port /dev/ttyUSB0 --baud 115200
```

La computadora se necesita para cargar cambios o leer el registro. Después se
puede usar un cargador o una batería USB regulada de 5 V que entregue corriente
suficiente para la placa. La alimentación del servo es independiente.

## Conexión del servo

Desconectar USB y fuente del servo antes de cambiar cables. La asignación supone
los colores habituales; confirmar la etiqueta o documentación del modelo.

| Cable | Conexión |
| --- | --- |
| Naranja / señal | **D5 de la NodeMCU = GPIO14**. No D1, no GPIO18. |
| Marrón / negativo | Negativo de la fuente del servo y cualquier **GND** de la NodeMCU. |
| Rojo / positivo | Positivo de una **fuente externa regulada** del voltaje indicado para el servo. SG90 continuo: fuente regulada de 5 V, con corriente suficiente para el motor. |

```text
Fuente del servo (+) ───────── rojo
Fuente del servo (−) ───┬───── marrón
                       └───── GND NodeMCU
                              D5 NodeMCU ───── naranja
USB 5 V ───────────────────── micro-USB NodeMCU
```

No unir el positivo de esa fuente con VIN/3V3 de la placa cuando la alimentas
por USB. La masa GND sí se comparte. No alimentar el servo desde 3V3 ni desde
un GPIO. La salida D5 es una señal lógica de 3.3 V; verificar que el servo
acepte esa señal. Este servo es **SG90 de rotación continua**. No interpreta 0/90/180 como
posiciones físicas. Esta versión estima el desplazamiento por tiempo y requiere
calibración. Si zumba, se atasca o llega a un tope, detener y revisar el montaje.
En variantes NodeMCU V3, VV/VU suele llevar los 5 V del USB: esa tensión no
garantiza corriente suficiente para el servo. Si se alimenta desde ahí para
una prueba breve y la placa se reinicia, desconectar el servo y usar su fuente
externa; no conectar además otra fuente positiva a VV/VIN con el USB conectado.

## Pines y conexiones que evitar

- **Cortocircuito:** nunca unir directamente un positivo de alimentación
  (5 V/VIN/3V3) con GND, ni invertir rojo y marrón. Esto sí puede causar daño.
- **Sobretensión:** no aplicar 5 V a 3V3, a un GPIO, a A0, EN o RST. Los GPIO
  son de 3.3 V. La escala de A0 depende de la placa; no se usa aquí.
- **EN/CH_PD y RST:** son habilitación y reinicio, no alimentación del servo.
- **D3/GPIO0, D4/GPIO2 y D8/GPIO15:** evitar para este servo porque seleccionan
  el modo de arranque. Usarlos mal puede impedir iniciar; no es necesariamente
  un cortocircuito.
- **SD0/SD1/SD2/SD3/CMD/CLK (GPIO6–11):** reservados para memoria flash.
- **TX/GPIO1 y RX/GPIO3:** reservar para programación y monitor serial.
- **VIN/VU:** el circuito de alimentación cambia entre variantes NodeMCU;
  este montaje alimenta la placa por micro-USB y no necesita esos pines.

Para este proyecto conectar a la placa únicamente la señal a D5 y el negativo
común a GND; alimentar el rojo desde la fuente del servo.

## Uso desde iPhone: servo continuo

1. Encender la placa y conectar el teléfono a `PopPopBoat`, clave `12345678`.
2. Abrir `http://192.168.4.1` y recargar para sustituir la página anterior.
3. Abrir **Calibrar el servo continuo**. Trabajar con el eje libre, sin
   llevar el mecanismo contra topes.
4. Empezar por **Probar parada**: aplica la señal neutra durante 1,5 s.
   El valor inicial es 1500 µs. Si gira, variar unos pocos µs y repetir hasta
   encontrar el valor que lo deja quieto.
5. Marcar visualmente el eje y probar cada sentido por separado. Los 200 ms
   iniciales son un valor de prueba, **no una medición de 90°**. Ajustar cada
   duración hasta que la prueba recorra 90°. Si se puede medir el ángulo:
   `tiempo nuevo = tiempo usado × 90 / ángulo observado`. Repetir la prueba
   para verificar. Las velocidades pueden diferir entre sentidos.
6. Marcar la casilla de confirmación y **Guardar calibración en la placa**.
   El neutro y los dos tiempos quedan en memoria EEPROM emulada.
7. Alinear físicamente el timón al centro usando los pulsos cortos de 40 ms,
   sin forzar el eje con la mano, y pulsar **Está en 90°**.
8. Usar los botones **0°**, **90°** y **180°**. Son destinos estimados:
   el firmware calcula la duración desde la posición estimada anterior.
   Los botones de posición permanecen bloqueados mientras hay un giro.

**Detener movimiento** interrumpe una prueba o un recorrido. Al parar, el
firmware escribe el neutro configurado y suspende los pulsos PWM; no conserva
una orden de velocidad distinta de cero. Al encender tampoco se emiten pulsos
hasta una orden explícita. Una prueba dura como máximo 5 segundos; un recorrido
completo de 180° puede durar el doble del tiempo calibrado para 90°. El fin del
recorrido se programa en el microcontrolador y no depende de que el navegador
permanezca abierto. No es un freno mecánico ni una parada eléctrica de emergencia.

La calibración queda guardada, pero **el centro se marca después de cada
encendido**. Los pulsos de ajuste o prueba invalidan la referencia; después de
ellos hay que volver a marcar 90°. Cambiar la calibración también invalida la
referencia. Si se detiene un giro a medias, el siguiente recorrido parte de la
posición intermedia estimada por el tiempo transcurrido.

No hay sensor de posición ni topes controlados. La carga, aceleración, inercia
y tensión introducen errores que se acumulan. Este programa **no garantiza**
que el mecanismo se mantenga físicamente entre 0° y 180°. Repetir la referencia
si aparece desfase. Para posiciones fiables y mantenidas se necesita un servo
posicional o un sistema con realimentación.

La versión posicional anterior está en
`backups/esp8266-positional-before-continuous.tar.gz`.

## Verificación de la lógica sin placa

```sh
gcc -std=c11 -Wall -Wextra -Werror -I main tests/servo_control_test.c main/main.c -o /tmp/poppopboat-servo-test
/tmp/poppopboat-servo-test
```

Referencias técnicas:
- [Librerías ESP8266 y alimentación del servo](https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#servo)
- [Pines que seleccionan el arranque](https://docs.espressif.com/projects/esptool/en/latest/esp8266/advanced-topics/boot-mode-selection.html)
- [Diseño eléctrico del ESP8266](https://www.espressif.com/sites/default/files/documentation/esp8266_hardware_design_guidelines_en.pdf)
- [Esquema original NodeMCU](https://github.com/nodemcu/nodemcu-devkit-v1.0)

- [SG90 continuo: ficha del fabricante](https://towerpro.com.tw/product/sg90-360-degree-continuous-rotation-servo/)
