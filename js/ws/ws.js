'use strict';

const WebSocket = require('ws');
const WebSocketServer = require('ws').Server;
const dotenv = require('dotenv');

dotenv.load();

function onMessage(ws, message) {
    console.log(message);
}

function onError(ws, message) {
    if (ws) {
        console.log("Error: " + message);
        var error = {
            code: Response.Error,
            message: message
        };
        var res = JSON.stringify(error);
        ws.send(res);
    }
}

function attemptServerConnection() {
    var masterWebSocketServerAddress = process.env.MASTER_WEBSOCKET_SERVER_ADDRESS;
    var masterWebSocketServerPort = process.env.MASTER_WEBSOCKET_SERVER_PORT;

    if (!masterWebSocketServerAddress) {
        masterWebSocketServerAddress = 'ws://localhost';
    }

    if (!masterWebSocketServerPort) {
        masterWebSocketServerPort = "8081";
    }

    var wsAddress = [masterWebSocketServerAddress, masterWebSocketServerPort].join(":");

    console.log("Connecting...", wsAddress);
    
    var client = new WebSocket(wsAddress, null, {perMessageDeflate: true});
    client.onerror = (err) => {};
    client.onopen = (event) => {
        console.log("Found connection at", wsAddress);
    };
    client.onmessage = (event) => {
        try {
            if (event.data) {
                var data = JSON.parse(event.data);
                onMessage(client, data);
            }
        } catch (e) {
            console.log(e);
            onError(ws, "Could not parse event. ");
        }
    };
    client.onclose = (event) => {
        console.log("Disconnected.")
        setTimeout(attemptServerConnection, 500);    
    };
}

attemptServerConnection();
