'use strict';
const http = require('http');
const dgram = require('dgram');
const fs = require('fs');
const path = require('path');
const os = require('os');

const files = new Map([
    ['/', ['index.html', 'text/html']],
    ['/stylesheets/style.css', ['stylesheets/style.css', 'text/css']],
    ['/javascripts/beatbox_ui.js', ['javascripts/beatbox_ui.js', 'application/javascript']]
]);

function relay(command) {
    return new Promise((resolve, reject) => {
        const socket = dgram.createSocket('udp4');
        const timer = setTimeout(() => finish(new Error('No beatbox reply on UDP port 12345')), 1000);
        let done = false;
        function finish(error, value) {
            if (done) return;
            done = true;
            clearTimeout(timer);
            socket.close();
            if (error) reject(error);
            else resolve(value);
        }
        socket.once('error', finish);
        socket.once('message', data => finish(null, data.toString()));
        socket.connect(12345, '127.0.0.1', () => socket.send(command));
    });
}

http.createServer(async (req, res) => {
    const url = new URL(req.url, 'http://localhost');
    res.setHeader('Cache-Control', 'no-store');
    try {
        if (req.method === 'GET' && files.has(url.pathname)) {
            const [file, mime] = files.get(url.pathname);
            const data = await fs.promises.readFile(path.join(__dirname, 'public', file));
            res.writeHead(200, {'Content-Type': mime});
            return res.end(data);
        }
        if (req.method === 'GET' && url.pathname === '/api/status') {
            const [mode, volume, tempo] = await Promise.all(['mode', 'volume', 'tempo'].map(relay));
            res.writeHead(200, {'Content-Type': 'application/json'});
            return res.end(JSON.stringify({mode, volume, tempo, uptime: Math.floor(os.uptime())}));
        }
        if (req.method === 'POST' && url.pathname === '/api/command') {
            const cmd = url.searchParams.get('cmd') || '';
            if (!/^(stop|(mode|volume|tempo|play) [0-9]{1,3})$/.test(cmd)) {
                res.writeHead(400);
                return res.end('Invalid command');
            }
            const reply = await relay(cmd);
            res.writeHead(reply.startsWith('error') ? 400 : 200);
            return res.end(reply);
        }
        res.writeHead(404);
        res.end();
    } catch (error) {
        res.writeHead(503);
        res.end(error.message);
    }
}).listen(8088, () => console.log('Beatbox web server listening on port 8088'));
