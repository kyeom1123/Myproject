# vSomeIP Demo (Server + Client)

This minimal project runs a vSomeIP **server** and **client** on your Linux PC (Ubuntu 22.04).
- Service: `0x0420`
- Instance: `0x0001`
- Method (Echo): `0x0001` (TCP)
- Event: `0x8001` in EventGroup `0x0001` (UDP)

## 0) Install dependencies & vSomeIP
```bash
sudo apt update
sudo apt install -y git build-essential cmake libboost-all-dev
git clone https://github.com/GENIVI/vsomeip.git
cd vsomeip && mkdir build && cd build
cmake -DENABLE_SIGNAL_HANDLING=1 ..
make -j$(nproc)
sudo make install
sudo ldconfig
```

## 1) Build this project
```bash
cd vsomeip-demo
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 2) Run
Open two terminals.

**Terminal 1 — Server**
```bash
cd vsomeip-demo
export VSOMEIP_APPLICATION_NAME=demo_server
export VSOMEIP_CONFIGURATION=$(pwd)/config/server.json
./build/someip_server
```

**Terminal 2 — Client**
```bash
cd vsomeip-demo
export VSOMEIP_APPLICATION_NAME=demo_client
export VSOMEIP_CONFIGURATION=$(pwd)/config/client.json
./build/someip_client
```

You should see:
- Client: `availability: UP` → periodic `[CLIENT] send echo` → echo responses
- Client: periodic event logs from server
- Server: request logs and event publish logs

## 3) Customizing to your controller's ARXML
Edit both `config/*.json`:
- `service`, `instance`, `major`, `minor`
- `methods[].id`, `events[].id`, `events[].eventgroup`
- `reliable` / `unreliable` (event transport)
- If not on localhost, change `unicast` to your NIC IP.
- Optionally pin ports: `reliable_server_port`, `unreliable_server_port`, and client `service-discovery.client_port`.

## 4) VS Code
Use the provided **.vscode** launch configurations:
- Build via *Terminal* → `cmake -S . -B build && cmake --build build -j`
- Launch "Run someip_server" and "Run someip_client" (opens external consoles)
```