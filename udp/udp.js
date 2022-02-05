const os = require("os");
const dgram = require("dgram");

const DEVICE_DISCOVERY_PORT = 12345;
const RECEIVE_PORT = 23456;

let udpClients = [];

function handleDiscoverRequest(msg, remoteInfo) {
    console.log("received " + msg.toString("ascii"), remoteInfo);
    let unicastSocket = dgram.createSocket({
        type: 'udp4',
        reuseAddr: true
    });
    unicastSocket.send("Hello", RECEIVE_PORT, remoteInfo.address, (err, bytes) => {
        unicastSocket.close();
    });
}

function createListeners() {
    for (let u of udpClients) {
        u.socket.close();
    }
    udpClients = [];
    const networkInterfaces = os.networkInterfaces();
    Object.keys(networkInterfaces).forEach(index => {
        const networkInterfaceList = networkInterfaces[index];
        for (const networkInterface of networkInterfaceList) {
            if (networkInterface.family === "IPv4" && networkInterface.internal === false) {
                const udpClient = {};
                udpClient.socket = dgram.createSocket({
                    type: 'udp4',
                    reuseAddr: true
                }, (msg, remoteInfo) => {
                    handleDiscoverRequest(msg, remoteInfo);
                });
                udpClient.socket.bind(DEVICE_DISCOVERY_PORT);
                udpClients.push(udpClient);
            }
        }
    });
}

setInterval(createListeners, 2500);