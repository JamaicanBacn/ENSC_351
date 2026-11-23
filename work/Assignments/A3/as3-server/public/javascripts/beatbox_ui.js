'use strict';
const el = id => document.getElementById(id);
let state;
let pending = Promise.resolve();
let queued = 0;
let stopped = false;

async function request(url, options) {
    const response = await fetch(url, options);
    if (!response.ok) throw new Error(await response.text());
    return response;
}

async function refresh() {
    state = await (await request('/api/status')).json();
    el('modeid').textContent = ['None', 'Rock', 'Custom'][state.mode];
    el('volumeid').value = state.volume;
    el('tempoid').value = state.tempo;
    el('status').textContent = 'Device uptime: ' + state.uptime + ' seconds';
}

async function update(command) {
    if (stopped) return;
    if (typeof command === 'function') command = command();
    try {
        if (command) await request('/api/command?cmd=' + encodeURIComponent(command), {method: 'POST'});
        if (command === 'stop') {
            stopped = true;
            el('status').textContent = 'Beatbox stopped';
        } else {
            await refresh();
        }
        el('error-box').hidden = true;
    } catch (error) {
        el('error-text').textContent = error.message;
        el('error-box').hidden = false;
    }
}

function enqueue(command) {
    if (!command && queued) return;
    queued++;
    pending = pending.then(() => update(command)).finally(() => queued--);
}

const actions = {modeNone: 'mode 0', modeRock1: 'mode 1', modeRock2: 'mode 2',
    base: 'play 0', 'hi-hat': 'play 1', snare: 'play 2', stop: 'stop'};
for (const [id, command] of Object.entries(actions)) {
    el(id).onclick = () => enqueue(command);
}
for (const [key, low, high] of [['tempo', 40, 300], ['volume', 0, 100]]) {
    for (const [suffix, delta] of [['Up', 5], ['Down', -5]]) {
        el(key + suffix).onclick = () => {
            if (state) enqueue(() => key + ' ' + Math.min(high, Math.max(low, Number(state[key]) + delta)));
        };
    }
}
el('error-box').hidden = true;
enqueue();
setInterval(() => enqueue(), 1000);
