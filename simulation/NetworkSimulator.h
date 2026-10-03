#ifndef NETWORKSIMULATOR_H
#define NETWORKSIMULATOR_H
#include "../core/Host.h"
#include "../core/Router.h"
#include "../core/Switch.h"
#include "../core/NetworkLink.h"
#include "../core/Packet.h"
#include "../core/EthernetFrame.h"
#include "../core/ARPPacket.h"
#include "../core/UDPDatagram.h"
#include "../core/TCPSegment.h"
#include "../core/TCPConnection.h"
#include "../core/TCPCongestionControl.h"
#include "../core/NetworkQueue.h"
#include "../core/Subnet.h"
#include "../core/Firewall.h"
#include "../core/IDS.h"
#include "../core/NetworkMetrics.h"
#include "../routing/Dijkstra.h"
#include "../routing/BellmanFord.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
struct PacketForwardingResult{bool delivered;bool dropped;bool expired;double totalLatency;int hops;std::vector<NetworkNode*> path;std::string reason;};
struct UDPTransmissionResult{bool delivered;bool checksumValid;double totalLatency;int hops;int sourcePort;int destinationPort;std::string payload;std::string reason;std::vector<NetworkNode*> path;};
struct TCPTransmissionResult{bool delivered;bool checksumValid;double totalLatency;int hops;int sourcePort;int destinationPort;uint32_t sequenceNumber;uint32_t acknowledgmentNumber;uint16_t windowSize;uint8_t flags;std::string payload;std::string reason;std::vector<NetworkNode*> path;};
struct TCPHandshakeResult{bool success;double totalLatency;int hops;uint32_t clientSequence;uint32_t serverSequence;uint32_t clientAcknowledgment;uint32_t serverAcknowledgment;TCPState clientState;TCPState serverState;std::string reason;};
struct TCPReliableTransmissionResult{bool connectionEstablished;bool delivered;bool acknowledged;int retransmissions;double totalLatency;int hops;uint32_t sequenceNumber;uint32_t acknowledgmentNumber;std::string payload;TCPState finalClientState;TCPState finalServerState;std::string reason;};
struct TCPTerminationResult{bool terminated;double totalLatency;int hops;TCPState clientState;TCPState serverState;std::string reason;};
struct TCPCongestionResult{bool connectionEstablished;bool completed;int totalSegments;int acknowledgedSegments;int lostSegments;int retransmissions;double totalLatency;double finalCongestionWindow;double finalSlowStartThreshold;std::string finalState;std::vector<double> congestionWindowHistory;std::vector<std::string> stateHistory;std::string reason;};
struct QueueSimulationResult{bool completed;int packetsGenerated;int packetsEnqueued;int packetsDequeued;int packetsDropped;double totalQueueDelay;double averageQueueDelay;double maximumQueueSize;double utilization;std::string reason;};
struct FailureSimulationResult{bool beforeFailureDelivered;bool afterFailureDelivered;bool recoveredDelivered;double beforeLatency;double afterLatency;double recoveredLatency;int beforeHops;int afterHops;int recoveredHops;std::vector<NetworkNode*> beforePath;std::vector<NetworkNode*> afterPath;std::vector<NetworkNode*> recoveredPath;std::string failureType;std::string reason;};
struct FirewallTransmissionResult{bool firewallAllowed;bool delivered;int priority;double totalLatency;int hops;std::string protocol;std::string reason;std::vector<NetworkNode*> path;};
struct IDSTransmissionResult{bool firewallAllowed;bool delivered;bool anomalyDetected;int alertCount;double totalLatency;int hops;std::string protocol;std::string reason;std::vector<NetworkNode*> path;};
class NetworkSimulator{
private:
    std::vector<std::unique_ptr<NetworkNode>> nodes;
    std::vector<std::unique_ptr<NetworkLink>> links;
    Firewall firewall;
    IDS ids;
    NetworkMetrics metrics;
    int nextNodeId;
    int nextPacketId;
    NetworkNode* findNodeByIP(const IPv4Address& ip) const;
    NetworkNode* findNeighbor(NetworkNode* node,NetworkNode* target) const;
    NetworkNode* findNeighborByIP(NetworkNode* node,const IPv4Address& ip) const;
    NetworkLink* findLink(NetworkNode* a,NetworkNode* b) const;
public:
    NetworkSimulator();
    Host* addHost(const std::string& name,const std::string& mac);
    Router* addRouter(const std::string& name);
    Switch* addSwitch(const std::string& name);
    NetworkLink* connect(NetworkNode* a,NetworkNode* b,double bandwidth,double latency,double packetLoss=0.0);
    Packet createPacket(NetworkNode* source,NetworkNode* destination,int size,const std::string& protocol);
    EthernetFrame createFrame(Host* source,const std::string& destinationMAC,const Packet& packet);
    ARPPacket createARPRequest(Host* source,Host* destination);
    ARPPacket createARPReply(Host* source,Host* destination);
    void processARPRequest(Host* sender,Host* target,Switch* sw,int incomingPort);
    void processARPReply(Host* sender,Host* target,Switch* sw,int incomingPort);
    bool resolveMAC(Host* source,Host* destination,Switch* sw,int sourcePort,int destinationPort);
    void sendFrame(Host* source,Host* destination,Switch* sw,int sourcePort,int destinationPort);
    PathResult calculatePath(NetworkNode* source,NetworkNode* destination);
    void displayPath(NetworkNode* source,NetworkNode* destination);
    void buildRoutingTables();
    BellmanFordResult calculateDistanceVector(NetworkNode* source);
    void displayDistanceVector(NetworkNode* source);
    void buildDistanceVectorTables();
    void buildBellmanFordRoutingTables();
    PacketForwardingResult forwardPacket(NetworkNode* source,NetworkNode* destination,int size,const std::string& protocol);
    PacketForwardingResult forwardPacket(NetworkNode* source,const IPv4Address& destinationIP,int size,const std::string& protocol);
    UDPTransmissionResult sendUDP(NetworkNode* source,NetworkNode* destination,int sourcePort,int destinationPort,const std::string& payload);
    UDPTransmissionResult sendUDP(NetworkNode* source,const IPv4Address& destinationIP,int sourcePort,int destinationPort,const std::string& payload);
    TCPTransmissionResult sendTCP(NetworkNode* source,NetworkNode* destination,int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload);
    TCPTransmissionResult sendTCP(NetworkNode* source,const IPv4Address& destinationIP,int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload);
    TCPHandshakeResult establishTCPConnection(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,uint32_t clientSequence,uint32_t serverSequence,uint16_t windowSize);
    TCPReliableTransmissionResult sendReliableTCP(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,const std::string& payload,uint32_t sequenceNumber,uint16_t windowSize,int maxRetransmissions=3);
    TCPTerminationResult terminateTCPConnection(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,uint32_t clientSequence,uint32_t serverSequence,uint16_t windowSize);
    TCPCongestionResult simulateTCPCongestion(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,int totalSegments,uint16_t windowSize=65535,double lossProbability=0.0);
    QueueSimulationResult simulateNetworkQueue(Router* router,const std::string& interfaceName,int packetCount,int packetSize,const std::string& protocol,size_t queueCapacity);
    void failLink(NetworkNode* a,NetworkNode* b);
    void recoverLink(NetworkNode* a,NetworkNode* b);
    void failNode(NetworkNode* node);
    void recoverNode(NetworkNode* node);
    bool isPathAvailable(NetworkNode* source,NetworkNode* destination);
    FailureSimulationResult simulateFailure(NetworkNode* source,NetworkNode* destination,NetworkNode* failureA,NetworkNode* failureB);
    Firewall& getFirewall();
    FirewallTransmissionResult sendThroughFirewall(NetworkNode* source,NetworkNode* destination,const std::string& protocol,int sourcePort,int destinationPort,int size);
    IDS& getIDS();
    IDSTransmissionResult sendWithIDS(NetworkNode* source,NetworkNode* destination,const std::string& protocol,int sourcePort,int destinationPort,int size);
    NetworkMetrics& getMetrics();
    void resetMetrics();
    void displayMetrics() const;
    void displayForwardingResult(const PacketForwardingResult& result) const;
    void displayUDPResult(const UDPTransmissionResult& result) const;
    void displayTCPResult(const TCPTransmissionResult& result) const;
    void displayTCPHandshakeResult(const TCPHandshakeResult& result) const;
    void displayTCPReliableResult(const TCPReliableTransmissionResult& result) const;
    void displayTCPTerminationResult(const TCPTerminationResult& result) const;
    void displayTCPCongestionResult(const TCPCongestionResult& result) const;
    void displayQueueSimulationResult(const QueueSimulationResult& result) const;
    void displayFailureSimulationResult(const FailureSimulationResult& result) const;
    void displayFirewallResult(const FirewallTransmissionResult& result) const;
    void displayIDSResult(const IDSTransmissionResult& result) const;
    void displayARPTable(Host* host) const;
    void displayTopology() const;
    void displayNodes() const;
    void displayLinks() const;
    bool sameSubnet(NetworkNode* a,NetworkNode* b) const;
};
#endif
