const os = require("os");
const dgram = require("dgram");
const Netmask = require("netmask").Netmask;

const DEVICE_DISCOVERY_PORT = 12345;
const BROADCAST_HEADER = [0xAA, 0xBB, 0xCC, 0xDD];
const BROADCAST_HEADER_LEN = 4;
let udpClients = [];

async function getDiscoverabilityData(versionNumber) {
    let deviceId = "whatever123";
    const broadcastData = {
        version: versionNumber,
        deviceName: os.hostname(),
        deviceId
    };
    const broadcastDataStr = JSON.stringify(broadcastData);
    const broadcastDataBuff = Buffer.alloc(Buffer.byteLength(broadcastDataStr), broadcastDataStr);
    const bufArray = Uint8Array.prototype.slice.call(broadcastDataBuff);
    const bufWithHeader = new Uint8Array(broadcastDataBuff.length + BROADCAST_HEADER_LEN);
    bufWithHeader.set(BROADCAST_HEADER, 0);
    bufWithHeader.set(bufArray, BROADCAST_HEADER_LEN);
    return bufWithHeader;
}

async function startDiscoverabilityBroadcast(versionNumber) {
    broadcastBuffer = await getDiscoverabilityData(versionNumber);
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
                } /*, (msg: Buffer, remoteInfo: dgram.RemoteInfo) => {
                    udpClient.messageReceived = true;
                    udpClient.message = Buffer.from(msg);
                    udpClient.remoteInfo = remoteInfo;
                }*/);
                udpClient.socket.on('error', (err) => {
                    console.log(`socket error:\n${err.stack}`);
                    udpClient.socket.close();
                });
                var netmask = new Netmask(networkInterface.cidr);
                udpClient.address = "255.255.255.255"; //netmask.broadcast;
                udpClient.socket.bind(() => {
                    udpClient.socket.setBroadcast(true);    
                    udpClients.push(udpClient);
                });
            }
        }
    });

    const BROADCAST_INTERVAL_MS = 1000;
    setInterval(() => {
        for (const udpClient of udpClients) {
            udpClient.socket.send(broadcastBuffer, DEVICE_DISCOVERY_PORT, udpClient.address, (err, bytes) => {
                if (err) {
                    console.error(err);
                } else {
                    console.log("Broadcast sent..." + udpClient.address);
                }
            });
        }
    }, BROADCAST_INTERVAL_MS);
}
startDiscoverabilityBroadcast("0.0.1");
