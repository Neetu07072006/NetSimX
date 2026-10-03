# NetSimX — Advanced Computer Network Simulator & Security Platform

NetSimX is an advanced computer network simulation and monitoring platform developed using C++, Node.js, Express and React.

The project simulates a complete network environment containing hosts, routers, switches, links, packet forwarding, routing algorithms, transport protocols, queues, firewall rules, intrusion detection and network monitoring.

A React dashboard provides a visual interface for observing the simulation, analyzing network traffic, monitoring security events and running individual network experiments.

## Features

### Network Core

- Host simulation
- Router simulation
- Switch simulation
- Network links
- IPv4 addressing
- Subnet handling
- MAC addressing
- Ethernet frames
- Packet creation and forwarding

### Address Resolution

- ARP request generation
- ARP reply generation
- ARP cache
- MAC resolution
- Ethernet frame forwarding

### Routing

- Routing tables
- Dijkstra shortest-path routing
- Link-state routing
- Bellman-Ford distance-vector routing
- Dynamic route calculation
- Alternate path handling

### Transport Protocols

- UDP datagrams
- TCP segments
- TCP connection establishment
- TCP three-way handshake
- TCP reliable transmission
- TCP retransmission
- TCP connection termination
- TCP congestion control
- Segment loss simulation

### Network Queues

- Router queues
- Queue capacity
- Packet enqueue/dequeue
- Queue drops
- Queue delay
- Queue utilization
- Maximum queue size

### Network Failure Simulation

- Link failure
- Link recovery
- Node failure
- Node recovery
- Alternate route detection
- Failure impact analysis

### Network Security

- Stateful firewall simulation
- Allow/deny rules
- Protocol filtering
- Port filtering
- Source/destination filtering
- Firewall logging
- Intrusion Detection System
- Repeated denial detection
- Port-scan detection
- Traffic-spike detection
- Suspicious protocol detection
- Multiple-attack detection
- Alert severity classification

### Monitoring

NetSimX tracks:

- Total packets
- Delivered packets
- Dropped packets
- Expired packets
- Total bytes
- Total latency
- Average latency
- Throughput
- Packet-loss rate
- Firewall activity
- IDS alerts
- Queue drops
- Queue delay
- Queue utilization
- Link failures
- Recovery events
- Protocol-level traffic

## Technology Stack

| Layer | Technology |
|---|---|
| Network Simulation | C++ |
| Routing Algorithms | C++ |
| Security Engine | C++ |
| Metrics Engine | C++ |
| Backend API | Node.js |
| REST API | Express.js |
| Frontend | React |
| Build Tool | Vite |
| Visualization | Recharts |
| Data Exchange | JSON |
| Version Control | Git/GitHub |

## Architecture

```text
                         ┌─────────────────────────┐
                         │      React Dashboard    │
                         │                         │
                         │ Overview                │
                         │ Topology                │
                         │ Routing                 │
                         │ Traffic                 │
                         │ Security                │
                         │ Metrics                 │
                         │ Simulation Lab          │
                         └────────────┬────────────┘
                                      │
                                      │ REST API
                                      ▼
                         ┌─────────────────────────┐
                         │      Express Server     │
                         │                         │
                         │ /api/snapshot           │
                         │ /api/metrics            │
                         │ /api/security           │
                         │ /api/topology           │
                         │ /api/routing            │
                         │ /api/run-simulation     │
                         └────────────┬────────────┘
                                      │
                                      │
                                      ▼
                         ┌─────────────────────────┐
                         │     C++ Simulation      │
                         │                         │
                         │ NetworkSimulator        │
                         │ Packet Forwarding       │
                         │ Routing                 │
                         │ TCP / UDP               │
                         │ Queues                  │
                         │ Firewall                │
                         │ IDS                     │
                         │ NetworkMetrics          │
                         └────────────┬────────────┘
                                      │
                                      ▼
                         ┌─────────────────────────┐
                         │ netsim_snapshot.json    │
                         └─────────────────────────┘