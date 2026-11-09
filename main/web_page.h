#pragma once
#include <pgmspace.h>

static const char WEB_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<meta name="theme-color" content="#edf3f2">
<title>PopPopBoat · Timón</title>
<style>
:root{color-scheme:light;--ink:#153c3d;--muted:#627c7c;--teal:#146c65;--line:#dce7e4;--fill:50%}
*{box-sizing:border-box}html{background:#edf3f2;-webkit-text-size-adjust:100%}
body{margin:0;color:var(--ink);font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;min-height:100vh;min-height:100svh;background:radial-gradient(ellipse at top right,#d9e9e3,transparent 55%)}
.app{width:100%;max-width:620px;margin:auto;padding:calc(24px + env(safe-area-inset-top)) max(20px,env(safe-area-inset-right)) calc(24px + env(safe-area-inset-bottom)) max(20px,env(safe-area-inset-left))}
header{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-bottom:24px}.brand{display:flex;gap:10px;align-items:center}.mark{width:42px;height:42px;padding:9px;border:1px solid #cbdcd6;border-radius:14px;background:#f8fbf8}.brand strong{display:block;font-size:19px;letter-spacing:-.7px}.eyebrow{display:block;font-size:10px;letter-spacing:1.6px;font-weight:700;color:var(--muted);margin-top:4px}
.connection{display:flex;align-items:center;gap:6px;font-size:12px;color:var(--muted);white-space:nowrap}.dot{width:7px;height:7px;flex:none;border-radius:50%;background:#9aaead}.connection[data-state=online]{color:#247359}.connection[data-state=online] .dot{background:#278b64}.connection[data-state=offline]{color:#ad463b}.connection[data-state=offline] .dot{background:#c75c50}
h1{font-size:32px;letter-spacing:-1.3px;line-height:1.1;margin:0 0 8px}p{margin:0}.intro{color:var(--muted);font-size:15px;line-height:1.5;margin-bottom:22px}.card{background:#fff;border:1px solid #e0eae5;border-radius:28px;padding:24px;box-shadow:0 12px 35px #173a3810}.card-top{display:flex;align-items:center;justify-content:space-between;gap:10px}.label{font-size:12px;font-weight:600;color:var(--muted)}.mode{font-size:11px;font-weight:700;color:var(--teal);background:#edf6ef;border-radius:20px;padding:7px 10px}
.dial{position:relative;max-width:300px;height:190px;margin:8px auto 0}.dial svg{width:100%;height:100%;overflow:visible}.needle{transform-origin:140px 140px;transition:transform .12s ease-out}.reading{position:absolute;inset:auto 0 5px;text-align:center}.reading output{font-size:76px;letter-spacing:-4px;font-weight:650;line-height:1}.degree{font-size:32px;vertical-align:top;display:inline-block;margin-top:7px;margin-left:3px}.sent{text-align:center;color:var(--muted);font-size:12px;margin:8px 0 18px;font-variant-numeric:tabular-nums}
.slider-label{display:block;font-size:13px;font-weight:600;margin-bottom:4px}input[type=range]{display:block;width:100%;margin:0;height:48px;background:transparent;accent-color:var(--teal);cursor:pointer;-webkit-appearance:none;appearance:none;touch-action:pan-y}input[type=range]::-webkit-slider-runnable-track{height:7px;border-radius:8px;background:linear-gradient(to right,var(--teal) var(--fill),#e4eeea var(--fill))}input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:30px;height:30px;border:4px solid #fff;background:var(--teal);border-radius:50%;margin-top:-11.5px;box-shadow:0 2px 8px #123b3a40}input[type=range]::-moz-range-track{height:7px;border-radius:8px;background:#e4eeea}input[type=range]::-moz-range-progress{height:7px;border-radius:8px;background:var(--teal)}input[type=range]::-moz-range-thumb{width:22px;height:22px;border:4px solid #fff;border-radius:50%;background:var(--teal);box-shadow:0 2px 8px #123b3a40}.ticks{display:flex;justify-content:space-between;color:var(--muted);font-size:11px;margin:0 2px 20px}
button{font:600 16px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;border:0;border-radius:14px;min-height:52px;cursor:pointer;touch-action:manipulation;transition:background .15s,transform .1s;min-width:0;-webkit-tap-highlight-color:transparent}button:active{transform:scale(.97)}button:disabled{opacity:.45;cursor:default}button:focus-visible,input:focus-visible{outline:3px solid #d5a55d;outline-offset:4px}.manual{display:grid;grid-template-columns:1fr 1.2fr 1fr;gap:8px}.step{background:#eff4f1;color:var(--ink)}.center{background:var(--teal);color:white}.hint{text-align:center;font-size:11px;color:var(--muted);line-height:1.4;margin-top:14px}
.auto-card{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-top:14px;padding:20px;border:1px solid #dce6e1;border-radius:22px;background:#f7faf6}.auto-card h2{margin:0 0 5px;font-size:15px;letter-spacing:-.3px}.auto-card p{font-size:12px;line-height:1.4;color:var(--muted)}.auto-button{flex:none;padding:0 17px;background:#e0ece5;color:var(--ink);font-size:14px;min-height:48px}.auto-button[aria-pressed=true]{background:var(--teal);color:white}.stop{display:block;width:100%;margin-top:14px;background:#173e3d;color:#fff;min-height:56px}.stop small{display:block;font-size:10px;font-weight:400;opacity:.75;margin-top:4px}.message{min-height:36px;padding:12px 4px 0;text-align:center;font-size:12px;line-height:1.5;color:var(--muted)}footer{display:flex;justify-content:space-between;gap:10px;border-top:1px solid #d6e2dc;padding-top:16px;margin-top:16px;font-size:11px;color:var(--muted);font-variant-numeric:tabular-nums}
.presets{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px;margin-top:20px}.preset{background:#edf4ef;color:var(--ink);min-height:66px;font-size:23px}.preset[aria-pressed=true]{background:var(--teal);color:white}.reference{padding:0 13px;font-size:13px;background:#e0ece5;color:var(--ink);flex:none}.calibration{margin-top:18px;background:#fff;border:1px solid var(--line);border-radius:22px;padding:20px}.calibration summary{font-weight:650;font-size:16px;cursor:pointer;min-height:32px}.calibration p{font-size:13px;line-height:1.55;color:var(--muted);margin:10px 0}.fields{display:grid;grid-template-columns:1fr 1fr;gap:12px}.field{display:block;font-size:12px;font-weight:600;color:var(--muted);margin:12px 0}.field input{display:block;width:100%;min-width:0;height:48px;padding:10px;margin-top:7px;font:16px -apple-system,sans-serif;color:var(--ink);background:#f7faf8;border:1px solid #cdded5;border-radius:10px}.cal-buttons{display:grid;grid-template-columns:1fr 1fr;gap:8px}.cal-button{font-size:13px;background:#edf4ef;color:var(--ink);padding:8px;min-height:48px}.wide{width:100%;margin-top:8px}.confirm{display:flex;align-items:flex-start;gap:10px;font-size:13px;line-height:1.5;margin:16px 0}.confirm input{width:20px;height:20px;flex:none;accent-color:var(--teal)}.save{background:var(--teal);color:white;width:100%;font-size:14px}.note{font-size:12px;line-height:1.5;color:var(--muted);margin-top:16px}.stop{position:sticky;bottom:max(10px,env(safe-area-inset-bottom));z-index:2;box-shadow:0 4px 18px #153c3d25}
@media(min-width:600px){.app{padding-top:40px}.card{padding:30px}.dial{height:210px}.intro{margin-bottom:26px}}
@media(max-width:359px){.app{padding-left:14px;padding-right:14px}.card{padding:18px}.brand strong{font-size:17px}.connection{font-size:11px}.auto-card{padding:16px}.auto-button{padding:0 12px}}
@media(prefers-reduced-motion:reduce){*{transition:none!important}}
</style>
</head>
<body>
<main class="app">
<header>
  <div class="brand"><svg class="mark" viewBox="0 0 24 24" fill="none" aria-hidden="true"><path d="M4 14h16l-3 5H7l-3-5ZM12 3v9H5l7-9Z" stroke="#146c65" stroke-width="1.6" stroke-linejoin="round"/><path d="M3 22c2-2 4 2 6 0s4 2 6 0 4 2 6 0" stroke="#146c65" stroke-width="1.5" stroke-linecap="round"/></svg><div><strong>PopPopBoat</strong><span class="eyebrow">CONTROL DEL TIMÓN</span></div></div>
  <div class="connection" id="connection" data-state="waiting" role="status"><span class="dot"></span><span id="connection-text">Conectando</span></div>
</header>
<h1>El rumbo, en tus manos.</h1>
<p class="intro">Tres rumbos, con giro calibrado por tiempo.</p>
<section class="card" aria-label="Control de posición">
  <div class="card-top"><span class="label">Posición estimada</span><span class="mode" id="mode">Sin referencia</span></div>
  <div class="dial">
    <svg viewBox="0 0 280 180" fill="none" aria-hidden="true"><path d="M30 140a110 110 0 0 1 220 0" stroke="#e8f0eb" stroke-width="13" stroke-linecap="round"/><path d="M30 140a110 110 0 0 1 110-110" stroke="#c7ded2" stroke-width="13" stroke-linecap="round"/><g class="needle" id="needle"><path d="m140 26-6 15h12l-6-15Z" fill="#146c65"/></g></svg>
    <div class="reading"><output id="angle">—</output><span class="degree">°</span></div>
  </div>
  <p class="sent" id="sent">Calibra el giro y marca el centro.</p>
  <div class="presets" aria-label="Elegir posición estimada">
    <button type="button" class="preset" data-position="0" aria-pressed="false" disabled>0°</button>
    <button type="button" class="preset" data-position="90" aria-pressed="false" disabled>90°</button>
    <button type="button" class="preset" data-position="180" aria-pressed="false" disabled>180°</button>
  </div>
  <p class="hint">Estimaciones por tiempo, sin sensor de posición.</p>
</section>
<section class="auto-card" aria-label="Referencia inicial"><div><h2>Marcar el centro</h2><p>Alinea el timón y confirma 90°.<br>Repite al encender la placa.</p></div><button type="button" id="reference" class="reference" disabled>Está en 90°</button></section>
<button type="button" id="stop" class="stop" disabled>Detener movimiento<small>Interrumpe cualquier giro o prueba</small></button>
<p class="message" id="message" role="status">Buscando el barco en la red PopPopBoat…</p>
<details class="calibration" id="calibration" open>
  <summary>1. Calibrar el servo continuo</summary>
  <p>Haz estas pruebas con el eje libre y sin llevar el timón a un tope. Los tiempos iniciales no están medidos para tu servo.</p>
  <label class="field" for="neutral">Señal de parada (µs)<input id="neutral" type="number" inputmode="numeric" min="1300" max="1700" step="1" value="1500"></label>
  <button type="button" class="cal-button wide" id="test-neutral" disabled>Probar parada durante 1,5 s</button>
  <p>Si gira durante esa prueba, ajusta la parada unos pocos µs y repite hasta que quede quieto.</p>
  <div class="fields">
    <label class="field" for="zero-ms">Tiempo hacia 0° (ms)<input id="zero-ms" type="number" inputmode="numeric" min="20" max="5000" step="1" value="200"></label>
    <label class="field" for="end-ms">Tiempo hacia 180° (ms)<input id="end-ms" type="number" inputmode="numeric" min="20" max="5000" step="1" value="200"></label>
  </div>
  <div class="cal-buttons"><button type="button" class="cal-button" id="test-zero" disabled>Probar hacia 0°</button><button type="button" class="cal-button" id="test-end" disabled>Probar hacia 180°</button></div>
  <p>Marca una referencia visual en el eje. Ajusta cada tiempo hasta que su prueba recorra 90°: menos tiempo gira menos; más tiempo gira más. Si puedes medir el ángulo, tiempo nuevo = tiempo usado × 90 / ángulo observado.</p>
  <label class="confirm"><input id="confirmed" type="checkbox"><span>Verifiqué la parada y medí un giro de 90° en ambos sentidos.</span></label>
  <button type="button" class="save" id="save" disabled>Guardar calibración en la placa</button>
  <p id="saved">La referencia del centro no se guarda: se marca después de cada encendido.</p>
  <p><strong>2. Alinear el centro</strong></p>
  <p>Usa los pulsos cortos para orientar el timón al centro, sin forzar el eje con la mano. Después pulsa «Está en 90°».</p>
  <div class="cal-buttons"><button type="button" class="cal-button" id="nudge-zero" disabled>Pulso hacia 0° · 40 ms</button><button type="button" class="cal-button" id="nudge-end" disabled>Pulso hacia 180° · 40 ms</button></div>
</details>
<p class="note">La posición puede desviarse con la carga y la alimentación. Vuelve a marcar el centro si notas un desfase. Un servo continuo no garantiza límites físicos de 0° a 180°.</p>
<footer><span>PopPopBoat · WiFi local</span><span id="uptime">Encendido —</span></footer>
</main>
<script>
'use strict';
const $ = id => document.getElementById(id);
const presets = [...document.querySelectorAll('[data-position]')];
const actions = ['test-neutral', 'test-zero', 'test-end', 'nudge-zero', 'nudge-end', 'save'];
let state = null, online = false, busy = false, pending = null, revision = 0, initialized = false;

function enable() {
  presets.forEach(button => {
    button.disabled = !online || busy || !state || !state.calibrated || !state.referenced || state.moving;
  });
  $('stop').disabled = !online;
  $('reference').disabled = !online || busy || !state || !state.calibrated || state.moving;
  actions.forEach(id => $(id).disabled = !online || busy || !state || state.moving || (id === 'save' && !$('confirmed').checked));
  ['neutral', 'zero-ms', 'end-ms', 'confirmed'].forEach(id => $(id).disabled = busy || !!(state && state.moving));
}

function connection(value) {
  online = value;
  $('connection').dataset.state = value ? 'online' : 'offline';
  $('connection-text').textContent = value ? 'Conectado' : 'Sin conexión';
  if (!value) $('message').textContent = 'Reconecta tu iPhone a PopPopBoat. El giro termina por su temporizador.';
  enable();
}

function render(next) {
  state = next;
  if (!initialized) {
    $('neutral').value = next.stop_us;
    $('zero-ms').value = next.to_zero_ms;
    $('end-ms').value = next.to_180_ms;
    $('calibration').open = !next.calibrated;
    initialized = true;
  }
  $('angle').value = next.referenced ? String(next.angle) : '—';
  $('needle').style.transform = 'rotate(' + ((next.referenced ? next.angle : 90) - 90) + 'deg)';
  $('needle').style.opacity = next.referenced ? '1' : '.25';
  $('mode').textContent = next.testing ? 'Prueba' : next.moving ? 'Moviendo' : next.referenced ? 'Parado' : 'Sin referencia';
  $('sent').textContent = next.testing ? 'Prueba limitada: ' + next.remaining_ms + ' ms restantes' : next.moving ? 'Destino estimado: ' + next.target + '°' : next.referenced ? 'Posición estimada por tiempo' : 'El centro físico todavía no está marcado.';
  presets.forEach(button => button.setAttribute('aria-pressed', String(next.referenced && Number(button.dataset.position) === next.target)));
  $('saved').textContent = next.calibrated ? 'Calibración disponible. Marca el centro después de cada encendido.' : 'Calibración pendiente. Los tiempos iniciales son solo para probar.';
  $('message').textContent = !next.calibrated ? 'Paso 1: calibra la parada y los tiempos de giro.' : !next.referenced ? 'Paso 2: alinea el timón y pulsa «Está en 90°».' : next.moving ? 'Puedes interrumpir el giro con Detener.' : 'Listo. Elige 0°, 90° o 180°.';
  $('uptime').textContent = 'Encendido ' + Math.floor(next.uptime_s / 60) + ':' + String(next.uptime_s % 60).padStart(2, '0');
  enable();
}

async function request(url, method = 'GET') {
  const controller = new AbortController();
  const timer = setTimeout(() => controller.abort(), 3500);
  try {
    const response = await fetch(url, {method, cache: 'no-store', signal: controller.signal});
    if (!response.ok) throw new Error(await response.text());
    return await response.json();
  } finally { clearTimeout(timer); }
}

async function flush() {
  if (busy) return;
  busy = true;
  enable();
  try {
    while (pending) {
      const action = pending;
      pending = null;
      try {
        const result = await request(action.url, 'POST');
        if (action.revision === revision) { connection(true); render(result); }
      } catch (error) {
        if (action.revision === revision) {
          if (error.name === 'AbortError' || error instanceof TypeError) connection(false);
          $('message').textContent = error.message || 'No se pudo enviar la orden.';
        }
      }
    }
  } finally { busy = false; enable(); }
}

function command(url) {
  pending = {url, revision: ++revision};
  void flush();
}

function calibrationQuery() {
  const fields = [['neutral', 1300, 1700], ['zero-ms', 20, 5000], ['end-ms', 20, 5000]];
  const values = fields.map(([id, min, max]) => {
    const value = Number($(id).value);
    if (!Number.isInteger(value) || value < min || value > max) throw new Error('Revisa los valores de calibración.');
    return value;
  });
  return new URLSearchParams({stop_us: values[0], to_zero_ms: values[1], to_180_ms: values[2]});
}

function test(direction, duration) {
  try {
    const query = calibrationQuery();
    query.set('direction', direction);
    query.set('duration_ms', duration);
    command('/api/test?' + query);
  } catch (error) { $('message').textContent = error.message; }
}

presets.forEach(button => button.addEventListener('click', () => command('/api/angle?value=' + button.dataset.position)));
$('stop').addEventListener('click', () => command('/api/stop'));
$('reference').addEventListener('click', () => command('/api/reference'));
$('test-neutral').addEventListener('click', () => test(1, 1500));
$('test-zero').addEventListener('click', () => test(0, Number($('zero-ms').value)));
$('test-end').addEventListener('click', () => test(2, Number($('end-ms').value)));
$('nudge-zero').addEventListener('click', () => test(0, 40));
$('nudge-end').addEventListener('click', () => test(2, 40));
$('confirmed').addEventListener('change', enable);
['neutral', 'zero-ms', 'end-ms'].forEach(id => $(id).addEventListener('input', () => {
  $('confirmed').checked = false;
  enable();
}));
$('save').addEventListener('click', () => {
  try {
    if (!$('confirmed').checked) return;
    const query = calibrationQuery();
    query.set('confirmed', '1');
    command('/api/calibration?' + query);
  } catch (error) { $('message').textContent = error.message; }
});

async function poll() {
  if (!busy && !pending && !document.hidden) {
    const started = revision;
    try {
      const result = await request('/api/status');
      if (started === revision && !busy && !pending) { connection(true); render(result); }
    } catch (error) {
      if (started === revision && !busy && !pending) connection(false);
    }
  }
  setTimeout(poll, state && state.moving ? 150 : 500);
}
void poll();
</script>
</body>
</html>)HTML";
