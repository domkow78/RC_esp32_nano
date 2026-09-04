document.addEventListener("DOMContentLoaded", () => {
    const modeMap = {
        0: "BOOT",
        1: "INIT",
        2: "READY",
        3: "MANUAL",
        4: "SEMI_AUTO",
        5: "AUTO",
        6: "ERROR",
        7: "SAFE_STOP"
    };

    const state = {
        packets: 0,
        socket: null,
        reconnectMs: 1200
    };

    const el = {
        wsStatus: document.getElementById("ws-status"),
        lastUpdate: document.getElementById("last-update"),
        mode: document.getElementById("mode"),
        failsafe: document.getElementById("failsafe"),
        battery: document.getElementById("battery"),
        lidar: document.getElementById("lidar"),
        imu: document.getElementById("imu"),
        throttleCmd: document.getElementById("throttle-cmd"),
        steeringCmd: document.getElementById("steering-cmd"),
        escThrottle: document.getElementById("esc-throttle"),
        servoSteering: document.getElementById("servo-steering"),
        escArmed: document.getElementById("esc-armed"),
        escBrake: document.getElementById("esc-brake"),
        wsServer: document.getElementById("ws-server"),
        packetCount: document.getElementById("packet-count")
    };

    function setConnection(online) {
        el.wsStatus.textContent = online ? "Online" : "Offline";
        el.wsStatus.classList.toggle("chip-online", online);
        el.wsStatus.classList.toggle("chip-offline", !online);
    }

    function asText(value, fallback) {
        return value === undefined || value === null ? fallback : String(value);
    }

    function asNumber(value, digits) {
        if (typeof value !== "number" || Number.isNaN(value)) {
            return "-";
        }
        return value.toFixed(digits);
    }

    function updateUi(payload) {
        state.packets += 1;
        el.packetCount.textContent = String(state.packets);
        el.lastUpdate.textContent = "Last packet: " + new Date().toLocaleTimeString();

        el.mode.textContent = modeMap[payload.mode] || "UNKNOWN";
        el.failsafe.textContent = payload.failsafe ? "ACTIVE" : "OK";
        el.battery.textContent = asNumber(payload.batteryV, 2) + " V";
        el.lidar.textContent = asNumber(payload.lidarCm, 1) + " cm";
        el.imu.textContent = asNumber(payload.imuHeading, 1) + " deg";
        el.throttleCmd.textContent = asText(payload.throttleCmd, "-");
        el.steeringCmd.textContent = asText(payload.steeringCmd, "-");
        el.escThrottle.textContent = asText(payload.escThrottle, "-");
        el.servoSteering.textContent = asText(payload.servoSteering, "-");
        el.escArmed.textContent = payload.escArmed ? "YES" : "NO";
        el.escBrake.textContent = payload.escBrake ? "ON" : "OFF";
        el.wsServer.textContent = "ACTIVE";
    }

    function connect() {
        const protocol = window.location.protocol === "https:" ? "wss" : "ws";
        const wsUrl = protocol + "://" + window.location.host + "/ws";
        const socket = new WebSocket(wsUrl);
        state.socket = socket;

        socket.addEventListener("open", () => {
            setConnection(true);
            el.wsServer.textContent = "ACTIVE";
        });

        socket.addEventListener("message", (event) => {
            try {
                const payload = JSON.parse(event.data);
                updateUi(payload);
            } catch {
                el.lastUpdate.textContent = "Packet parse error";
            }
        });

        socket.addEventListener("close", () => {
            setConnection(false);
            el.wsServer.textContent = "RECONNECTING";
            window.setTimeout(connect, state.reconnectMs);
        });

        socket.addEventListener("error", () => {
            socket.close();
        });
    }

    setConnection(false);
    connect();
});
