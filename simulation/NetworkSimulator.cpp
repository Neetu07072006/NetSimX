#include "NetworkSimulator.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include "../core/NetworkMetrics.h"
NetworkSimulator::NetworkSimulator()
    : nextNodeId(1),nextPacketId(1) {}
Host* NetworkSimulator::addHost(
    const std::string& name,
    const std::string& mac) {
    auto host=std::make_unique<Host>(
        nextNodeId++,
        name,
        mac
    );
    Host* ptr=host.get();
    nodes.push_back(std::move(host));
    return ptr;
}
Router* NetworkSimulator::addRouter(
    const std::string& name) {
    auto router=std::make_unique<Router>(
        nextNodeId++,
        name
    );
    Router* ptr=router.get();
    nodes.push_back(std::move(router));
    return ptr;
}
Switch* NetworkSimulator::addSwitch(
    const std::string& name) {
    auto sw=std::make_unique<Switch>(
        nextNodeId++,
        name
    );
    Switch* ptr=sw.get();
    nodes.push_back(std::move(sw));
    return ptr;
}
NetworkLink* NetworkSimulator::connect(
    NetworkNode* a,
    NetworkNode* b,
    double bandwidth,
    double latency,
    double packetLoss) {
    auto link=std::make_unique<NetworkLink>(
        a,
        b,
        bandwidth,
        latency,
        packetLoss
    );
    NetworkLink* ptr=link.get();
    links.push_back(std::move(link));
    a->addLink(ptr);
    b->addLink(ptr);
    return ptr;
}
Packet NetworkSimulator::createPacket(
    NetworkNode* source,
    NetworkNode* destination,
    int size,
    const std::string& protocol) {
    return Packet(
        nextPacketId++,
        source->getIPAddress(),
        destination->getIPAddress(),
        size,
        protocol
    );
}
EthernetFrame NetworkSimulator::createFrame(
    Host* source,
    const std::string& destinationMAC,
    const Packet& packet) {
    return EthernetFrame(
        source->getMACAddress(),
        destinationMAC,
        packet
    );
}
ARPPacket NetworkSimulator::createARPRequest(
    Host* source,
    Host* destination) {
    return ARPPacket(
        ARPOperation::REQUEST,
        source->getIPAddress(),
        source->getMACAddress(),
        destination->getIPAddress(),
        "FF:FF:FF:FF:FF:FF"
    );
}
ARPPacket NetworkSimulator::createARPReply(
    Host* source,
    Host* destination) {
    return ARPPacket(
        ARPOperation::REPLY,
        source->getIPAddress(),
        source->getMACAddress(),
        destination->getIPAddress(),
        destination->getMACAddress()
    );
}
void NetworkSimulator::processARPRequest(
    Host* sender,
    Host* target,
    Switch* sw,
    int incomingPort) {
    ARPPacket request=
        createARPRequest(
            sender,
            target
        );
    request.display();
    sw->learnMAC(
        sender->getMACAddress(),
        incomingPort
    );
    std::cout
        << "\nARP request broadcast.\n";
    if(target->getIPAddress()==
       request.getTargetIP()) {
        std::cout
            << "Target received ARP request.\n";
        processARPReply(
            target,
            sender,
            sw,
            2
        );
    }
}
void NetworkSimulator::processARPReply(
    Host* sender,
    Host* target,
    Switch* sw,
    int incomingPort) {
    ARPPacket reply=
        createARPReply(
            sender,
            target
        );
    reply.display();
    sw->learnMAC(
        sender->getMACAddress(),
        incomingPort
    );
    target->getARPCache().add(
        sender->getIPAddress(),
        sender->getMACAddress()
    );
    std::cout
        << "\nARP cache updated.\n";
    target->getARPCache().display();
}
bool NetworkSimulator::resolveMAC(
    Host* source,
    Host* destination,
    Switch* sw,
    int sourcePort,
    int destinationPort) {
    std::string cachedMAC=
        source->getARPCache().lookup(
            destination->getIPAddress()
        );
    if(!cachedMAC.empty()) {
        std::cout
            << "\nARP CACHE HIT\n";
        std::cout
            << "IP: "
            << destination->getIPAddress().toString()
            << '\n';
        std::cout
            << "MAC: "
            << cachedMAC
            << '\n';
        return true;
    }
    std::cout
        << "\nARP CACHE MISS\n";
    std::cout
        << "Starting ARP resolution...\n";
    ARPPacket request=
        createARPRequest(
            source,
            destination
        );
    request.display();
    sw->learnMAC(
        source->getMACAddress(),
        sourcePort
    );
    sw->learnMAC(
        destination->getMACAddress(),
        destinationPort
    );
    std::cout
        << "\nSwitch broadcasts ARP request.\n";
    ARPPacket reply=
        createARPReply(
            destination,
            source
        );
    reply.display();
    source->getARPCache().add(
        destination->getIPAddress(),
        destination->getMACAddress()
    );
    std::cout
        << "\nARP resolution completed.\n";
    source->getARPCache().display();
    return true;
}
void NetworkSimulator::sendFrame(
    Host* source,
    Host* destination,
    Switch* sw,
    int sourcePort,
    int destinationPort) {
    std::cout
        << "\n===== IP COMMUNICATION =====\n";
    std::cout
        << "Source      : "
        << source->getName()
        << " ("
        << source->getIPAddress().toString()
        << ")\n";
    std::cout
        << "Destination : "
        << destination->getName()
        << " ("
        << destination->getIPAddress().toString()
        << ")\n";
    if(!resolveMAC(
        source,
        destination,
        sw,
        sourcePort,
        destinationPort))
        return;
    std::string destinationMAC=
        source->getARPCache().lookup(
            destination->getIPAddress()
        );
    Packet packet=
        createPacket(
            source,
            destination,
            1024,
            "UDP"
        );
    EthernetFrame frame=
        createFrame(
            source,
            destinationMAC,
            packet
        );
    frame.display();
    sw->processFrame(
        frame,
        sourcePort
    );
    std::cout
        << "\n===== CURRENT MAC TABLE =====\n";
    sw->getMACTable().display();
}
PathResult NetworkSimulator::calculatePath(
    NetworkNode* source,
    NetworkNode* destination) {
    Dijkstra algorithm;
    std::vector<NetworkLink*> networkLinks;
    for(const auto& link:links)
        networkLinks.push_back(link.get());
    return algorithm.calculate(
        source,
        destination,
        networkLinks
    );
}
void NetworkSimulator::displayPath(
    NetworkNode* source,
    NetworkNode* destination) {
    PathResult result=
        calculatePath(
            source,
            destination
        );
    std::cout
        << "\n===== DIJKSTRA SHORTEST PATH =====\n";
    std::cout
        << "Source      : "
        << source->getName()
        << '\n';
    std::cout
        << "Destination : "
        << destination->getName()
        << '\n';
    if(result.path.empty()) {
        std::cout
            << "No path found.\n";
        return;
    }
    std::cout
        << "Path        : ";
    for(size_t i=0;
        i<result.path.size();
        ++i) {
        std::cout
            << result.path[i]->getName();
        if(i+1<result.path.size())
            std::cout
                << " -> ";
    }
    std::cout
        << '\n';
    std::cout
        << "Total Cost  : "
        << result.cost
        << " ms\n";
}
void NetworkSimulator::buildRoutingTables() {
    std::cout
        << "\n===== BUILDING ROUTING TABLES =====\n";
    std::vector<Router*> routers;
    for(const auto& node:nodes) {
        if(node->getType()==NodeType::ROUTER)
            routers.push_back(
                static_cast<Router*>(node.get())
            );
    }
    for(Router* router:routers) {
        router->clearRoutes();
        for(const auto& node:nodes) {
            NetworkNode* destination=node.get();
            if(destination==router)
                continue;
            PathResult result=
                calculatePath(
                    router,
                    destination
                );
            if(result.path.size()<2)
                continue;
            NetworkNode* nextHop=
                result.path[1];
            std::string interfaceName=
                "LINK-"+nextHop->getName();
            if(!destination->
                getIPAddress().isValid())
                continue;
            Subnet subnet(
                destination->getIPAddress(),
                24
            );
            router->addRoute(
                subnet.getNetworkAddress(),
                24,
                nextHop->getIPAddress(),
                interfaceName,
                static_cast<int>(
                    result.cost
                )
            );
        }
    }
}
BellmanFordResult
NetworkSimulator::calculateDistanceVector(
    NetworkNode* source) {
    BellmanFord algorithm;
    std::vector<NetworkLink*> networkLinks;
    for(const auto& link:links)
        networkLinks.push_back(link.get());
    return algorithm.calculate(
        source,
        networkLinks
    );
}
void NetworkSimulator::displayDistanceVector(
    NetworkNode* source) {
    BellmanFordResult result=
        calculateDistanceVector(
            source
        );
    std::cout
        << "\n===== BELLMAN-FORD DISTANCE VECTOR =====\n";
    std::cout
        << "Source Router : "
        << source->getName()
        << '\n';
    std::cout
        << "Iterations    : "
        << result.iterations
        << '\n';
    std::cout
        << "Converged     : "
        << (result.converged?"YES":"NO")
        << '\n';
    result.table.display();
}
void NetworkSimulator::buildDistanceVectorTables() {
    std::cout
        << "\n===== BUILDING DISTANCE VECTOR TABLES =====\n";
    for(const auto& node:nodes) {
        if(node->getType()!=NodeType::ROUTER)
            continue;
        Router* router=
            static_cast<Router*>(node.get());
        BellmanFordResult result=
            calculateDistanceVector(router);
        std::cout
            << "\nRouter "
            << router->getName()
            << " converged in "
            << result.iterations
            << " iterations.\n";
        result.table.display();
    }
}
void NetworkSimulator::buildBellmanFordRoutingTables() {
    std::cout
        << "\n===== BUILDING ROUTING TABLES USING BELLMAN-FORD =====\n";
    for(const auto& node:nodes) {
        if(node->getType()!=NodeType::ROUTER)
            continue;
        Router* router=
            static_cast<Router*>(node.get());
        router->clearRoutes();
        BellmanFordResult result=
            calculateDistanceVector(router);
        for(const auto& item:
            result.table.getEntries()) {
            const DistanceVectorEntry& entry=
                item.second;
            NetworkNode* destination=
                entry.destination;
            if(destination==router)
                continue;
            if(!destination->
                getIPAddress().isValid())
                continue;
            if(!entry.nextHop)
                continue;
            Subnet subnet(
                destination->getIPAddress(),
                24
            );
            std::string interfaceName=
                "LINK-"+entry.nextHop->getName();
            router->addRoute(
                subnet.getNetworkAddress(),
                24,
                entry.nextHop->getIPAddress(),
                interfaceName,
                static_cast<int>(
                    entry.cost
                )
            );
        }
    }
}
NetworkNode* NetworkSimulator::findNodeByIP(
    const IPv4Address& ip) const {
    for(const auto& node:nodes) {
        if(node->getIPAddress()==ip)
            return node.get();
    }
    return nullptr;
}
NetworkNode* NetworkSimulator::findNeighbor(
    NetworkNode* node,
    NetworkNode* target) const {
    for(NetworkLink* link:node->getLinks()) {
        if(link->getNodeA()==node &&
           link->getNodeB()==target)
            return target;
        if(link->getNodeB()==node &&
           link->getNodeA()==target)
            return target;
    }
    return nullptr;
}
NetworkNode* NetworkSimulator::findNeighborByIP(
    NetworkNode* node,
    const IPv4Address& ip) const {
    for(NetworkLink* link:node->getLinks()) {
        NetworkNode* neighbor=nullptr;
        if(link->getNodeA()==node)
            neighbor=link->getNodeB();
        if(link->getNodeB()==node)
            neighbor=link->getNodeA();
        if(neighbor &&
           neighbor->getIPAddress()==ip)
            return neighbor;
    }
    return nullptr;
}
NetworkLink* NetworkSimulator::findLink(
    NetworkNode* a,
    NetworkNode* b) const {
    for(const auto& link:links) {
        if((link->getNodeA()==a &&
            link->getNodeB()==b) ||
           (link->getNodeA()==b &&
            link->getNodeB()==a))
            return link.get();
    }
    return nullptr;
}
PacketForwardingResult
NetworkSimulator::forwardPacket(
    NetworkNode* source,
    NetworkNode* destination,
    int size,
    const std::string& protocol) {
    PacketForwardingResult result{
        false,
        false,
        false,
        0.0,
        0,
        {},
        ""
    };
    auto recordMetrics=[&](){metrics.recordPacket(size,result.delivered,result.dropped,result.expired,result.totalLatency);metrics.recordProtocolPacket(protocol,result.delivered,result.dropped);};
    if(!source ||
       !destination) {
        result.dropped=true;
        result.reason="Invalid source or destination.";
        recordMetrics();
        return result;
    }
    Packet packet=
        createPacket(
            source,
            destination,
            size,
            protocol
        );
    NetworkNode* current=source;
    result.path.push_back(current);
    const int maxHops=64;
    std::cout
        << "\n===== PACKET FORWARDING =====\n";
    packet.display();
    std::cout
        << "\nStarting forwarding from "
        << source->getName()
        << " to "
        << destination->getName()
        << ".\n";
    while(current!=destination) {
        if(result.hops>=maxHops) {
            result.dropped=true;
            result.reason="Maximum hop limit reached.";
            recordMetrics();
        return result;
        }
        NetworkNode* nextHop=nullptr;
        if(current->getType()==NodeType::HOST) {
            nextHop=
                findNeighbor(
                    current,
                    destination
                );
            if(!nextHop) {
                for(NetworkLink* link:
                    current->getLinks()) {
                    if(link->getNodeA()==current)
                        nextHop=link->getNodeB();
                    else if(link->getNodeB()==current)
                        nextHop=link->getNodeA();
                    if(nextHop)
                        break;
                }
            }
            if(!nextHop) {
                result.dropped=true;
                result.reason=
                    "Source host has no forwarding link.";
                recordMetrics();
        return result;
            }
        }
        else if(current->getType()==NodeType::ROUTER) {
            Router* router=
                static_cast<Router*>(current);
            const Route* route=
                router->lookupRoute(
                    destination->getIPAddress()
                );
            if(!route) {
                result.dropped=true;
                result.reason=
                    "No route to destination.";
                recordMetrics();
        return result;
            }
            nextHop=
                findNeighborByIP(
                    current,
                    route->getNextHop()
                );
            if(!nextHop) {
                nextHop=
                    findNodeByIP(
                        route->getNextHop()
                    );
            }
            if(!nextHop) {
                result.dropped=true;
                result.reason=
                    "Next hop is unreachable.";
                recordMetrics();
        return result;
            }
        }
        else {
            for(NetworkLink* link:
                current->getLinks()) {
                if(link->getNodeA()==current)
                    nextHop=link->getNodeB();
                else if(link->getNodeB()==current)
                    nextHop=link->getNodeA();
                if(nextHop)
                    break;
            }
            if(!nextHop) {
                result.dropped=true;
                result.reason=
                    "Network device has no forwarding link.";
                recordMetrics();
        return result;
            }
        }
        NetworkLink* link=
            findLink(
                current,
                nextHop
            );
        if(!link) {
            result.dropped=true;
            result.reason=
                "Network link not found.";
            recordMetrics();
        return result;
        }
        if(!link->isActive()) {
            result.dropped=true;
            result.reason=
                "Packet dropped because the network link is failed.";
            recordMetrics();
        return result;
        }
        if(link->getPacketLoss()>0.0) {
            result.dropped=true;
            result.reason=
                "Packet dropped due to simulated packet loss.";
            recordMetrics();
        return result;
        }
        result.totalLatency+=
            link->getLatency();
        ++result.hops;
        if(current->getType()==NodeType::ROUTER) {
            packet.decrementTTL();
            if(packet.isExpired()) {
                result.expired=true;
                result.dropped=true;
                result.reason=
                    "Packet TTL expired.";
                recordMetrics();
        return result;
            }
        }
        std::cout
            << "\nHop "
            << result.hops
            << ": "
            << current->getName()
            << " -> "
            << nextHop->getName()
            << " | Latency: "
            << link->getLatency()
            << " ms"
            << " | TTL: "
            << packet.getTTL()
            << '\n';
        current=nextHop;
        result.path.push_back(current);
    }
    result.delivered=true;
    result.reason="Packet delivered successfully.";
    recordMetrics();
        return result;
}
PacketForwardingResult NetworkSimulator::forwardPacket(NetworkNode* source,const IPv4Address& destinationIP,int size,const std::string& protocol){
    NetworkNode* destination=findNodeByIP(destinationIP);
    if(!destination){
        PacketForwardingResult result{false,true,false,0,0,{},"Destination IP does not exist."};
        metrics.recordPacket(size,false,true,false,0);
        metrics.recordProtocolPacket(protocol,false,true);
        return result;
    }
    return forwardPacket(source,destination,size,protocol);
}
UDPTransmissionResult NetworkSimulator::sendUDP(NetworkNode* source,NetworkNode* destination,int sourcePort,int destinationPort,const std::string& payload){
    UDPTransmissionResult result{false,false,0.0,0,sourcePort,destinationPort,payload,"",{}};
    UDPDatagram datagram(sourcePort,destinationPort,payload);
    if(!datagram.isValidPort()){
        result.reason="Invalid UDP port.";
        metrics.recordPacket(static_cast<long long>(payload.size()+8),false,true,false,0);
        metrics.recordProtocolPacket("UDP",false,true);
        return result;
    }
    if(!datagram.verifyChecksum()){
        result.reason="UDP checksum validation failed.";
        metrics.recordPacket(static_cast<long long>(payload.size()+8),false,true,false,0);
        metrics.recordProtocolPacket("UDP",false,true);
        return result;
    }
    datagram.display();
    PacketForwardingResult forwarding=forwardPacket(source,destination,datagram.getPayloadSize()+8,"UDP");
    result.totalLatency=forwarding.totalLatency;
    result.hops=forwarding.hops;
    result.path=forwarding.path;
    if(!forwarding.delivered){
        result.reason=forwarding.reason;
        return result;
    }
    result.delivered=true;
    result.checksumValid=datagram.verifyChecksum();
    result.reason="UDP datagram delivered successfully.";
    std::cout<<"\nUDP datagram delivered to port "<<destinationPort<<".\n";
    return result;
}
UDPTransmissionResult NetworkSimulator::sendUDP(NetworkNode* source,const IPv4Address& destinationIP,int sourcePort,int destinationPort,const std::string& payload){
    NetworkNode* destination=findNodeByIP(destinationIP);
    if(!destination){
        metrics.recordPacket(static_cast<long long>(payload.size()+8),false,true,false,0);metrics.recordProtocolPacket("UDP",false,true);
        return {false,false,0.0,0,sourcePort,destinationPort,payload,"Destination IP does not exist.",{}};
    }
    return sendUDP(source,destination,sourcePort,destinationPort,payload);
}
void NetworkSimulator::displayUDPResult(const UDPTransmissionResult& result) const{
    std::cout<<"\n===== UDP TRANSMISSION RESULT =====\n";
    std::cout<<"Status          : "<<(result.delivered?"DELIVERED":"FAILED")<<'\n';
    std::cout<<"Source Port     : "<<result.sourcePort<<'\n';
    std::cout<<"Destination Port: "<<result.destinationPort<<'\n';
    std::cout<<"Payload         : "<<result.payload<<'\n';
    std::cout<<"Payload Size    : "<<result.payload.size()<<" bytes\n";
    std::cout<<"Checksum        : "<<(result.checksumValid?"VALID":"INVALID")<<'\n';
    std::cout<<"Hops            : "<<result.hops<<'\n';
    std::cout<<"Total Latency   : "<<result.totalLatency<<" ms\n";
    std::cout<<"Path            : ";
    if(result.path.empty())std::cout<<"None";
    else{
        for(size_t i=0;i<result.path.size();++i){
            std::cout<<result.path[i]->getName();
            if(i+1<result.path.size())std::cout<<" -> ";
        }
    }
    std::cout<<'\n';
    std::cout<<"Reason          : "<<result.reason<<'\n';
}
void NetworkSimulator::displayForwardingResult(
    const PacketForwardingResult& result) const {
    std::cout
        << "\n===== FORWARDING RESULT =====\n";
    std::cout
        << "Status        : ";
    if(result.delivered)
        std::cout
            << "DELIVERED\n";
    else if(result.expired)
        std::cout
            << "TTL EXPIRED\n";
    else
        std::cout
            << "DROPPED\n";
    std::cout
        << "Hops          : "
        << result.hops
        << '\n';
    std::cout
        << "Total Latency : "
        << result.totalLatency
        << " ms\n";
    std::cout
        << "Path          : ";
    if(result.path.empty()) {
        std::cout
            << "None\n";
    }
    else {
        for(size_t i=0;
            i<result.path.size();
            ++i) {
            std::cout
                << result.path[i]->getName();
            if(i+1<result.path.size())
                std::cout
                    << " -> ";
        }
        std::cout
            << '\n';
    }
    std::cout
        << "Reason        : "
        << result.reason
        << '\n';
}
void NetworkSimulator::displayARPTable(
    Host* host) const {
    std::cout
        << "\n===== "
        << host->getName()
        << " ARP TABLE =====\n";
    host->getARPCache().display();
}
void NetworkSimulator::displayNodes() const {
    std::cout
        << "\n===== NETWORK NODES =====\n";
    for(const auto& node:nodes)
        node->display();
}
void NetworkSimulator::displayLinks() const {
    std::cout
        << "\n===== NETWORK LINKS =====\n";
    for(const auto& link:links)
        link->display();
}
void NetworkSimulator::displayTopology() const {
    displayNodes();
    displayLinks();
}
bool NetworkSimulator::sameSubnet(
    NetworkNode* a,
    NetworkNode* b) const {
    if(!a->getIPAddress().isValid() ||
       !b->getIPAddress().isValid())
        return false;
    Subnet subnet(
        a->getIPAddress(),
        24
    );
    return subnet.sameSubnet(
        b->getIPAddress()
    );
}
TCPTransmissionResult NetworkSimulator::sendTCP(NetworkNode* source,NetworkNode* destination,int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload){
    TCPSegment segment(sourcePort,destinationPort,sequenceNumber,acknowledgmentNumber,windowSize,flags,payload);
    TCPTransmissionResult result;
    result.delivered=false;
    result.checksumValid=segment.verifyChecksum();
    result.totalLatency=0;
    result.hops=0;
    result.sourcePort=sourcePort;
    result.destinationPort=destinationPort;
    result.sequenceNumber=sequenceNumber;
    result.acknowledgmentNumber=acknowledgmentNumber;
    result.windowSize=windowSize;
    result.flags=flags;
    result.payload=payload;
    result.reason="";
    if(!segment.isValidPort()){
        result.reason="Invalid TCP port number.";
        metrics.recordPacket(static_cast<long long>(payload.size()+20),false,true,false,0);
        metrics.recordProtocolPacket("TCP",false,true);
        return result;
    }
    if(!result.checksumValid){
        result.reason="TCP checksum validation failed.";
        metrics.recordPacket(static_cast<long long>(payload.size()+20),false,true,false,0);
        metrics.recordProtocolPacket("TCP",false,true);
        return result;
    }
    PacketForwardingResult forwarding=forwardPacket(source,destination,segment.getPayloadSize()+20,"TCP");
    result.delivered=forwarding.delivered;
    result.totalLatency=forwarding.totalLatency;
    result.hops=forwarding.hops;
    result.path=forwarding.path;
    result.reason=forwarding.reason;
    return result;
}
TCPTransmissionResult NetworkSimulator::sendTCP(NetworkNode* source,const IPv4Address& destinationIP,int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload){
    TCPSegment segment(sourcePort,destinationPort,sequenceNumber,acknowledgmentNumber,windowSize,flags,payload);
    TCPTransmissionResult result;
    result.delivered=false;
    result.checksumValid=segment.verifyChecksum();
    result.totalLatency=0;
    result.hops=0;
    result.sourcePort=sourcePort;
    result.destinationPort=destinationPort;
    result.sequenceNumber=sequenceNumber;
    result.acknowledgmentNumber=acknowledgmentNumber;
    result.windowSize=windowSize;
    result.flags=flags;
    result.payload=payload;
    result.reason="";
    if(!segment.isValidPort()){
        result.reason="Invalid TCP port number.";
        metrics.recordPacket(static_cast<long long>(payload.size()+20),false,true,false,0);
        metrics.recordProtocolPacket("TCP",false,true);
        return result;
    }
    if(!result.checksumValid){
        result.reason="TCP checksum validation failed.";
        metrics.recordPacket(static_cast<long long>(payload.size()+20),false,true,false,0);
        metrics.recordProtocolPacket("TCP",false,true);
        return result;
    }
    PacketForwardingResult forwarding=forwardPacket(source,destinationIP,segment.getPayloadSize()+20,"TCP");
    result.delivered=forwarding.delivered;
    result.totalLatency=forwarding.totalLatency;
    result.hops=forwarding.hops;
    result.path=forwarding.path;
    result.reason=forwarding.reason;
    return result;
}
void NetworkSimulator::displayTCPResult(const TCPTransmissionResult& result) const{
    std::cout<<"\n===== TCP TRANSMISSION RESULT =====\n";
    std::cout<<"Source Port          : "<<result.sourcePort<<'\n';
    std::cout<<"Destination Port     : "<<result.destinationPort<<'\n';
    std::cout<<"Sequence Number      : "<<result.sequenceNumber<<'\n';
    std::cout<<"Acknowledgment Number: "<<result.acknowledgmentNumber<<'\n';
    std::cout<<"Window Size          : "<<result.windowSize<<'\n';
    std::cout<<"Payload              : "<<result.payload<<'\n';
    std::cout<<"Checksum Valid       : "<<(result.checksumValid?"YES":"NO")<<'\n';
    std::cout<<"Delivered            : "<<(result.delivered?"YES":"NO")<<'\n';
    std::cout<<"Hops                 : "<<result.hops<<'\n';
    std::cout<<"Total Latency        : "<<result.totalLatency<<" ms\n";
    std::cout<<"Path                 : ";
    for(size_t i=0;i<result.path.size();++i){
        std::cout<<result.path[i]->getName();
        if(i+1<result.path.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
    std::cout<<"Reason               : "<<result.reason<<'\n';
}
TCPHandshakeResult NetworkSimulator::establishTCPConnection(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,uint32_t clientSequence,uint32_t serverSequence,uint16_t windowSize){
    TCPHandshakeResult result;
    result.success=false;
    result.totalLatency=0;
    result.hops=0;
    result.clientSequence=clientSequence;
    result.serverSequence=serverSequence;
    result.clientAcknowledgment=0;
    result.serverAcknowledgment=0;
    result.clientState=TCPState::CLOSED;
    result.serverState=TCPState::CLOSED;
    result.reason="";
    TCPConnection clientConnection(clientSequence,windowSize);
    TCPConnection serverConnection(serverSequence,windowSize);
    TCPSegment syn(clientPort,serverPort,clientSequence,0,windowSize,static_cast<uint8_t>(TCPFlag::SYN),"");
    PacketForwardingResult synResult=forwardPacket(client,server,20,"TCP");
    result.totalLatency+=synResult.totalLatency;
    result.hops+=synResult.hops;
    if(!synResult.delivered){result.reason="SYN packet was not delivered.";return result;}
    clientConnection.setState(TCPState::SYN_SENT);
    serverConnection.setState(TCPState::SYN_RECEIVED);
    serverConnection.setRemoteSequence(clientSequence);
    result.clientState=clientConnection.getState();
    result.serverState=serverConnection.getState();
    TCPSegment synAck(serverPort,clientPort,serverSequence,clientSequence+1,windowSize,static_cast<uint8_t>(TCPFlag::SYN)|static_cast<uint8_t>(TCPFlag::ACK),"");
    PacketForwardingResult synAckResult=forwardPacket(server,client,20,"TCP");
    result.totalLatency+=synAckResult.totalLatency;
    result.hops+=synAckResult.hops;
    if(!synAckResult.delivered){result.reason="SYN-ACK packet was not delivered.";return result;}
    clientConnection.setRemoteSequence(serverSequence);
    clientConnection.setState(TCPState::ESTABLISHED);
    result.clientAcknowledgment=serverSequence+1;
    TCPSegment ack(clientPort,serverPort,clientSequence+1,serverSequence+1,windowSize,static_cast<uint8_t>(TCPFlag::ACK),"");
    PacketForwardingResult ackResult=forwardPacket(client,server,20,"TCP");
    result.totalLatency+=ackResult.totalLatency;
    result.hops+=ackResult.hops;
    if(!ackResult.delivered){result.reason="ACK packet was not delivered.";return result;}
    serverConnection.setState(TCPState::ESTABLISHED);
    clientConnection.advanceLocalSequence(1);
    serverConnection.advanceLocalSequence(1);
    result.clientState=clientConnection.getState();
    result.serverState=serverConnection.getState();
    result.success=true;
    result.reason="TCP three-way handshake completed successfully.";
    return result;
}
TCPReliableTransmissionResult NetworkSimulator::sendReliableTCP(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,const std::string& payload,uint32_t sequenceNumber,uint16_t windowSize,int maxRetransmissions){
    TCPReliableTransmissionResult result;
    result.connectionEstablished=false;
    result.delivered=false;
    result.acknowledged=false;
    result.retransmissions=0;
    result.totalLatency=0;
    result.hops=0;
    result.sequenceNumber=sequenceNumber;
    result.acknowledgmentNumber=0;
    result.payload=payload;
    result.finalClientState=TCPState::CLOSED;
    result.finalServerState=TCPState::CLOSED;
    result.reason="";
    TCPHandshakeResult handshake=establishTCPConnection(client,server,clientPort,serverPort,sequenceNumber,2000,windowSize);
    result.totalLatency+=handshake.totalLatency;
    result.hops+=handshake.hops;
    result.connectionEstablished=handshake.success;
    result.finalClientState=handshake.clientState;
    result.finalServerState=handshake.serverState;
    if(!handshake.success){result.reason=handshake.reason;return result;}
    result.acknowledgmentNumber=2001;
    for(int attempt=0;attempt<=maxRetransmissions;++attempt){
        TCPTransmissionResult transmission=sendTCP(client,server,clientPort,serverPort,sequenceNumber,2001,windowSize,static_cast<uint8_t>(TCPFlag::ACK),payload);
        result.totalLatency+=transmission.totalLatency;
        result.hops+=transmission.hops;
        if(transmission.delivered&&transmission.checksumValid){
            result.delivered=true;
            TCPTransmissionResult acknowledgment=sendTCP(server,client,serverPort,clientPort,2001,sequenceNumber+static_cast<uint32_t>(payload.size()),windowSize,static_cast<uint8_t>(TCPFlag::ACK),"");
            result.totalLatency+=acknowledgment.totalLatency;
            result.hops+=acknowledgment.hops;
            if(acknowledgment.delivered&&acknowledgment.checksumValid){
                result.acknowledged=true;
                result.reason="TCP data delivered and acknowledged.";
                break;
            }
        }
        if(attempt<maxRetransmissions)++result.retransmissions;
    }
    result.finalClientState=TCPState::ESTABLISHED;
    result.finalServerState=TCPState::ESTABLISHED;
    if(!result.delivered)result.reason="TCP data delivery failed after retransmission attempts.";
    else if(!result.acknowledged)result.reason="TCP data delivered but acknowledgment failed.";
    return result;
}
TCPTerminationResult NetworkSimulator::terminateTCPConnection(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,uint32_t clientSequence,uint32_t serverSequence,uint16_t windowSize){
    TCPTerminationResult result;
    result.terminated=false;
    result.totalLatency=0;
    result.hops=0;
    result.clientState=TCPState::ESTABLISHED;
    result.serverState=TCPState::ESTABLISHED;
    result.reason="";
    TCPConnection clientConnection(clientSequence,windowSize);
    TCPConnection serverConnection(serverSequence,windowSize);
    clientConnection.setState(TCPState::ESTABLISHED);
    serverConnection.setState(TCPState::ESTABLISHED);
    TCPTransmissionResult fin=sendTCP(client,server,clientPort,serverPort,clientSequence,serverSequence+1,windowSize,static_cast<uint8_t>(TCPFlag::FIN)|static_cast<uint8_t>(TCPFlag::ACK),"");
    result.totalLatency+=fin.totalLatency;
    result.hops+=fin.hops;
    if(!fin.delivered){result.reason="FIN packet was not delivered.";return result;}
    clientConnection.setState(TCPState::FIN_WAIT_1);
    serverConnection.setState(TCPState::CLOSE_WAIT);
    TCPTransmissionResult ack=sendTCP(server,client,serverPort,clientPort,serverSequence+1,clientSequence+1,windowSize,static_cast<uint8_t>(TCPFlag::ACK),"");
    result.totalLatency+=ack.totalLatency;
    result.hops+=ack.hops;
    if(!ack.delivered){result.reason="FIN acknowledgment was not delivered.";return result;}
    clientConnection.setState(TCPState::FIN_WAIT_2);
    TCPTransmissionResult serverFin=sendTCP(server,client,serverPort,clientPort,serverSequence+1,clientSequence+1,windowSize,static_cast<uint8_t>(TCPFlag::FIN)|static_cast<uint8_t>(TCPFlag::ACK),"");
    result.totalLatency+=serverFin.totalLatency;
    result.hops+=serverFin.hops;
    if(!serverFin.delivered){result.reason="Server FIN was not delivered.";return result;}
    serverConnection.setState(TCPState::LAST_ACK);
    TCPTransmissionResult finalAck=sendTCP(client,server,clientPort,serverPort,clientSequence+1,serverSequence+2,windowSize,static_cast<uint8_t>(TCPFlag::ACK),"");
    result.totalLatency+=finalAck.totalLatency;
    result.hops+=finalAck.hops;
    if(!finalAck.delivered){result.reason="Final ACK was not delivered.";return result;}
    clientConnection.setState(TCPState::TIME_WAIT);
    serverConnection.setState(TCPState::CLOSED);
    result.clientState=clientConnection.getState();
    result.serverState=serverConnection.getState();
    result.terminated=true;
    result.reason="TCP connection terminated successfully.";
    return result;
}
void NetworkSimulator::displayTCPHandshakeResult(const TCPHandshakeResult& result) const{
    std::cout<<"\n===== TCP THREE-WAY HANDSHAKE =====\n";
    std::cout<<"SYN      : "<<(result.success?"DELIVERED":"FAILED")<<'\n';
    std::cout<<"SYN-ACK  : "<<(result.success?"DELIVERED":"FAILED")<<'\n';
    std::cout<<"ACK      : "<<(result.success?"DELIVERED":"FAILED")<<'\n';
    std::cout<<"Client Sequence      : "<<result.clientSequence<<'\n';
    std::cout<<"Server Sequence      : "<<result.serverSequence<<'\n';
    std::cout<<"Client ACK           : "<<result.clientAcknowledgment<<'\n';
    std::cout<<"Client State         : ";
    if(result.clientState==TCPState::ESTABLISHED)std::cout<<"ESTABLISHED\n";else std::cout<<"NOT ESTABLISHED\n";
    std::cout<<"Server State         : ";
    if(result.serverState==TCPState::ESTABLISHED)std::cout<<"ESTABLISHED\n";else std::cout<<"NOT ESTABLISHED\n";
    std::cout<<"Total Latency        : "<<result.totalLatency<<" ms\n";
    std::cout<<"Hops                 : "<<result.hops<<'\n';
    std::cout<<"Result               : "<<result.reason<<'\n';
}
void NetworkSimulator::displayTCPReliableResult(const TCPReliableTransmissionResult& result) const{
    std::cout<<"\n===== TCP RELIABLE TRANSMISSION =====\n";
    std::cout<<"Connection Established : "<<(result.connectionEstablished?"YES":"NO")<<'\n';
    std::cout<<"Data Delivered         : "<<(result.delivered?"YES":"NO")<<'\n';
    std::cout<<"Acknowledged           : "<<(result.acknowledged?"YES":"NO")<<'\n';
    std::cout<<"Sequence Number        : "<<result.sequenceNumber<<'\n';
    std::cout<<"Acknowledgment Number  : "<<result.acknowledgmentNumber<<'\n';
    std::cout<<"Payload                : "<<result.payload<<'\n';
    std::cout<<"Retransmissions        : "<<result.retransmissions<<'\n';
    std::cout<<"Total Latency          : "<<result.totalLatency<<" ms\n";
    std::cout<<"Hops                   : "<<result.hops<<'\n';
    std::cout<<"Client State           : ";
    if(result.finalClientState==TCPState::ESTABLISHED)std::cout<<"ESTABLISHED\n";else std::cout<<"CLOSED\n";
    std::cout<<"Server State           : ";
    if(result.finalServerState==TCPState::ESTABLISHED)std::cout<<"ESTABLISHED\n";else std::cout<<"CLOSED\n";
    std::cout<<"Result                 : "<<result.reason<<'\n';
}
void NetworkSimulator::displayTCPTerminationResult(const TCPTerminationResult& result) const{
    std::cout<<"\n===== TCP CONNECTION TERMINATION =====\n";
    std::cout<<"Terminated      : "<<(result.terminated?"YES":"NO")<<'\n';
    std::cout<<"Client State    : ";
    switch(result.clientState){
        case TCPState::TIME_WAIT:std::cout<<"TIME-WAIT";break;
        case TCPState::FIN_WAIT_1:std::cout<<"FIN-WAIT-1";break;
        case TCPState::FIN_WAIT_2:std::cout<<"FIN-WAIT-2";break;
        default:std::cout<<"OTHER";break;
    }
    std::cout<<'\n';
    std::cout<<"Server State    : ";
    switch(result.serverState){
        case TCPState::CLOSED:std::cout<<"CLOSED";break;
        case TCPState::LAST_ACK:std::cout<<"LAST-ACK";break;
        default:std::cout<<"OTHER";break;
    }
    std::cout<<'\n';
    std::cout<<"Total Latency   : "<<result.totalLatency<<" ms\n";
    std::cout<<"Hops            : "<<result.hops<<'\n';
    std::cout<<"Result          : "<<result.reason<<'\n';
}
TCPCongestionResult NetworkSimulator::simulateTCPCongestion(NetworkNode* client,NetworkNode* server,int clientPort,int serverPort,int totalSegments,uint16_t windowSize,double lossProbability){
    TCPCongestionResult result;
    result.connectionEstablished=false;
    result.completed=false;
    result.totalSegments=totalSegments;
    result.acknowledgedSegments=0;
    result.lostSegments=0;
    result.retransmissions=0;
    result.totalLatency=0;
    result.finalCongestionWindow=0;
    result.finalSlowStartThreshold=0;
    result.finalState="";
    result.reason="";
    if(totalSegments<=0){
        result.reason="Number of segments must be greater than zero.";
        return result;
    }
    TCPHandshakeResult handshake=establishTCPConnection(client,server,clientPort,serverPort,1000,2000,windowSize);
    result.totalLatency+=handshake.totalLatency;
    result.connectionEstablished=handshake.success;
    if(!handshake.success){
        result.reason=handshake.reason;
        return result;
    }
    TCPCongestionControl congestion(1.0,16.0,64.0);
    uint32_t sequence=3000;
    for(int segment=0;segment<totalSegments;){
        double currentWindow=congestion.getCongestionWindow();
        result.congestionWindowHistory.push_back(currentWindow);
        result.stateHistory.push_back(congestion.getState());
        int windowSegments=static_cast<int>(currentWindow);
        if(windowSegments<1)windowSegments=1;
        int remaining=totalSegments-segment;
        int sendCount=windowSegments<remaining?windowSegments:remaining;
        for(int i=0;i<sendCount;++i){
            bool lost=lossProbability>0.0&&((segment+i+1)%static_cast<int>(100/lossProbability)==0);
            if(lost){
                congestion.onTimeout();
                ++result.lostSegments;
                ++result.retransmissions;
                result.totalLatency+=10.0;
                result.congestionWindowHistory.push_back(congestion.getCongestionWindow());
                result.stateHistory.push_back(congestion.getState());
                continue;
            }
            std::string payload="TCP Segment "+std::to_string(segment+i+1);
            TCPTransmissionResult transmission=sendTCP(client,server,clientPort,serverPort,sequence,2001,windowSize,static_cast<uint8_t>(TCPFlag::ACK),payload);
            result.totalLatency+=transmission.totalLatency;
            if(transmission.delivered&&transmission.checksumValid){
                ++result.acknowledgedSegments;
                congestion.onACK();
                sequence+=static_cast<uint32_t>(payload.size());
            }else{
                congestion.onTimeout();
                ++result.lostSegments;
                ++result.retransmissions;
            }
        }
        segment+=sendCount;
        result.congestionWindowHistory.push_back(congestion.getCongestionWindow());
        result.stateHistory.push_back(congestion.getState());
    }
    result.finalCongestionWindow=congestion.getCongestionWindow();
    result.finalSlowStartThreshold=congestion.getSlowStartThreshold();
    result.finalState=congestion.getState();
    result.completed=result.acknowledgedSegments+result.lostSegments>=totalSegments;
    if(result.lostSegments==0)result.reason="All TCP segments delivered without simulated loss.";
    else result.reason="TCP congestion control responded to simulated packet loss.";
    return result;
}
void NetworkSimulator::displayTCPCongestionResult(const TCPCongestionResult& result) const{
    std::cout<<"\n===== TCP CONGESTION CONTROL RESULT =====\n";
    std::cout<<"Connection Established : "<<(result.connectionEstablished?"YES":"NO")<<'\n';
    std::cout<<"Simulation Completed   : "<<(result.completed?"YES":"NO")<<'\n';
    std::cout<<"Total Segments         : "<<result.totalSegments<<'\n';
    std::cout<<"Acknowledged Segments  : "<<result.acknowledgedSegments<<'\n';
    std::cout<<"Lost Segments          : "<<result.lostSegments<<'\n';
    std::cout<<"Retransmissions        : "<<result.retransmissions<<'\n';
    std::cout<<"Final cwnd             : "<<result.finalCongestionWindow<<" segments\n";
    std::cout<<"Final ssthresh         : "<<result.finalSlowStartThreshold<<" segments\n";
    std::cout<<"Final State            : "<<result.finalState<<'\n';
    std::cout<<"Total Latency          : "<<result.totalLatency<<" ms\n";
    std::cout<<"Congestion Window      : ";
    for(size_t i=0;i<result.congestionWindowHistory.size();++i){
        std::cout<<result.congestionWindowHistory[i];
        if(i+1<result.congestionWindowHistory.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
    std::cout<<"State History          : ";
    for(size_t i=0;i<result.stateHistory.size();++i){
        std::cout<<result.stateHistory[i];
        if(i+1<result.stateHistory.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
    std::cout<<"Result                 : "<<result.reason<<'\n';
}
QueueSimulationResult NetworkSimulator::simulateNetworkQueue(Router* router,const std::string& interfaceName,int packetCount,int packetSize,const std::string& protocol,size_t queueCapacity){
    QueueSimulationResult result;
    result.completed=false;
    result.packetsGenerated=packetCount;
    result.packetsEnqueued=0;
    result.packetsDequeued=0;
    result.packetsDropped=0;
    result.totalQueueDelay=0;
    result.averageQueueDelay=0;
    result.maximumQueueSize=0;
    result.utilization=0;
    result.reason="";
    if(router==nullptr){
        result.reason="Router is null.";
        return result;
    }
    if(packetCount<=0||packetSize<=0){
        result.reason="Packet count and packet size must be greater than zero.";
        return result;
    }
    router->createQueue(interfaceName,queueCapacity);
    NetworkQueue& queue=router->getQueueManager().getQueue(interfaceName);
    for(int i=0;i<packetCount;++i){
        QueuedPacket packet;
        packet.packetId=nextPacketId++;
        packet.size=packetSize;
        packet.protocol=protocol;
        packet.arrivalTime=static_cast<double>(i);
        packet.serviceTime=static_cast<double>(i+1);
        if(queue.enqueue(packet))++result.packetsEnqueued;
        else ++result.packetsDropped;
    }
    QueuedPacket packet;
    while(queue.dequeue(packet))++result.packetsDequeued;
    result.totalQueueDelay=queue.getTotalQueueDelay();
    result.averageQueueDelay=queue.getAverageQueueDelay();
    result.maximumQueueSize=static_cast<double>(queue.getMaximumSize());
    result.utilization=queue.getUtilization();
    result.completed=result.packetsDequeued==result.packetsEnqueued;
    metrics.recordQueue(result.packetsDropped,result.totalQueueDelay,result.maximumQueueSize,result.utilization);
    if(result.packetsDropped>0)result.reason="Queue overflow caused packet drops.";
    else result.reason="All packets were queued and transmitted successfully.";
    return result;
}
void NetworkSimulator::displayQueueSimulationResult(const QueueSimulationResult& result) const{
    std::cout<<"\n===== NETWORK QUEUE SIMULATION =====\n";
    std::cout<<"Completed          : "<<(result.completed?"YES":"NO")<<'\n';
    std::cout<<"Packets Generated  : "<<result.packetsGenerated<<'\n';
    std::cout<<"Packets Enqueued   : "<<result.packetsEnqueued<<'\n';
    std::cout<<"Packets Dequeued   : "<<result.packetsDequeued<<'\n';
    std::cout<<"Packets Dropped    : "<<result.packetsDropped<<'\n';
    std::cout<<"Maximum Queue Size : "<<result.maximumQueueSize<<'\n';
    std::cout<<"Total Queue Delay  : "<<result.totalQueueDelay<<" ms\n";
    std::cout<<"Average Queue Delay: "<<result.averageQueueDelay<<" ms\n";
    std::cout<<"Queue Utilization  : "<<result.utilization<<"%\n";
    std::cout<<"Result             : "<<result.reason<<'\n';
}
void NetworkSimulator::failLink(NetworkNode* a,NetworkNode* b){
    NetworkLink* link=findLink(a,b);
    if(link&&link->isActive()){
        link->fail();
        metrics.recordLinkFailure();
    }
}
void NetworkSimulator::recoverLink(NetworkNode* a,NetworkNode* b){
    NetworkLink* link=findLink(a,b);
    if(link&&!link->isActive()){
        link->recover();
        metrics.recordRecovery();
    }
}
void NetworkSimulator::failNode(NetworkNode* node){
    if(node==nullptr)return;
    for(NetworkLink* link:node->getLinks()){
        if(link->isActive()){
            link->fail();
            metrics.recordLinkFailure();
        }
    }
}
void NetworkSimulator::recoverNode(NetworkNode* node){
    if(node==nullptr)return;
    for(NetworkLink* link:node->getLinks()){
        if(!link->isActive()){
            link->recover();
            metrics.recordRecovery();
        }
    }
}
bool NetworkSimulator::isPathAvailable(NetworkNode* source,NetworkNode* destination){
    PathResult result=calculatePath(source,destination);
    return !result.path.empty()&&result.path.front()==source&&result.path.back()==destination;
}
FailureSimulationResult NetworkSimulator::simulateFailure(NetworkNode* source,NetworkNode* destination,NetworkNode* failureA,NetworkNode* failureB){
    FailureSimulationResult result{};
    PathResult before=calculatePath(source,destination);
    result.beforeFailureDelivered=!before.path.empty()&&before.path.front()==source&&before.path.back()==destination;
    result.beforeLatency=result.beforeFailureDelivered?before.cost:0;
    result.beforeHops=result.beforeFailureDelivered?static_cast<int>(before.path.size())-1:0;
    result.beforePath=before.path;
    NetworkLink* firstLink=findLink(failureA,failureB);
    if(firstLink){
        failLink(failureA,failureB);
        result.failureType="LINK FAILURE";
    }else{
        failNode(failureA);
        result.failureType="NODE FAILURE";
    }
    PathResult after=calculatePath(source,destination);
    result.afterFailureDelivered=!after.path.empty()&&after.path.front()==source&&after.path.back()==destination;
    result.afterLatency=result.afterFailureDelivered?after.cost:0;
    result.afterHops=result.afterFailureDelivered?static_cast<int>(after.path.size())-1:0;
    result.afterPath=after.path;
    if(result.afterFailureDelivered)result.reason="Traffic remains reachable after failure and uses the available path.";
    else result.reason="No active path remains between source and destination.";
    if(firstLink)recoverLink(failureA,failureB);else recoverNode(failureA);
    PathResult recovered=calculatePath(source,destination);
    result.recoveredDelivered=!recovered.path.empty()&&recovered.path.front()==source&&recovered.path.back()==destination;
    result.recoveredLatency=result.recoveredDelivered?recovered.cost:0;
    result.recoveredHops=result.recoveredDelivered?static_cast<int>(recovered.path.size())-1:0;
    result.recoveredPath=recovered.path;
    return result;
}
void NetworkSimulator::displayFailureSimulationResult(const FailureSimulationResult& result) const{
    std::cout<<"\n===== FAILURE SIMULATION =====\n";
    std::cout<<"Failure Type          : "<<result.failureType<<'\n';
    std::cout<<"Before Failure        : "<<(result.beforeFailureDelivered?"DELIVERED":"UNREACHABLE")<<'\n';
    std::cout<<"Before Latency        : "<<result.beforeLatency<<" ms\n";
    std::cout<<"Before Hops           : "<<result.beforeHops<<'\n';
    std::cout<<"After Failure         : "<<(result.afterFailureDelivered?"DELIVERED":"UNREACHABLE")<<'\n';
    std::cout<<"After Latency         : "<<result.afterLatency<<" ms\n";
    std::cout<<"After Hops            : "<<result.afterHops<<'\n';
    std::cout<<"Recovered Network     : "<<(result.recoveredDelivered?"DELIVERED":"UNREACHABLE")<<'\n';
    std::cout<<"Recovered Latency     : "<<result.recoveredLatency<<" ms\n";
    std::cout<<"Recovered Hops        : "<<result.recoveredHops<<'\n';
    std::cout<<"Reason                : "<<result.reason<<'\n';
    std::cout<<"Before Path           : ";
    for(size_t i=0;i<result.beforePath.size();++i){
        std::cout<<result.beforePath[i]->getName();
        if(i+1<result.beforePath.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
    std::cout<<"After Path            : ";
    if(result.afterPath.empty())std::cout<<"NONE";
    for(size_t i=0;i<result.afterPath.size();++i){
        std::cout<<result.afterPath[i]->getName();
        if(i+1<result.afterPath.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
    std::cout<<"Recovered Path        : ";
    for(size_t i=0;i<result.recoveredPath.size();++i){
        std::cout<<result.recoveredPath[i]->getName();
        if(i+1<result.recoveredPath.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
}
NetworkMetrics& NetworkSimulator::getMetrics(){return metrics;}
void NetworkSimulator::resetMetrics(){metrics.reset();}
void NetworkSimulator::displayMetrics() const{metrics.display();}
Firewall& NetworkSimulator::getFirewall(){return firewall;}
FirewallTransmissionResult NetworkSimulator::sendThroughFirewall(NetworkNode* source,NetworkNode* destination,const std::string& protocol,int sourcePort,int destinationPort,int size){
    FirewallDecision decision=firewall.evaluate(source->getIPAddress(),destination->getIPAddress(),protocol,sourcePort,destinationPort);
    metrics.recordFirewall(decision.allowed);
    FirewallTransmissionResult result{};
    result.firewallAllowed=decision.allowed;
    result.priority=decision.priority;
    result.protocol=protocol;
    result.reason=decision.reason;
    if(!decision.allowed){
        metrics.recordPacket(size,false,true,false,0);
        metrics.recordProtocolPacket(protocol,false,true);
        result.delivered=false;
        result.totalLatency=0;
        result.hops=0;
        return result;
    }
    PacketForwardingResult forwarding=forwardPacket(source,destination,size,protocol);
    result.delivered=forwarding.delivered;
    result.totalLatency=forwarding.totalLatency;
    result.hops=forwarding.hops;
    result.path=forwarding.path;
    if(!forwarding.delivered)result.reason=forwarding.reason;
    else result.reason="Firewall allowed the traffic and packet was delivered.";
    return result;
}
void NetworkSimulator::displayFirewallResult(const FirewallTransmissionResult& result) const{
    std::cout<<"\n===== FIREWALL TRANSMISSION =====\n";
    std::cout<<"Protocol          : "<<result.protocol<<'\n';
    std::cout<<"Firewall Decision : "<<(result.firewallAllowed?"ALLOW":"DENY")<<'\n';
    std::cout<<"Rule Priority     : "<<result.priority<<'\n';
    std::cout<<"Packet Delivered  : "<<(result.delivered?"YES":"NO")<<'\n';
    std::cout<<"Total Latency     : "<<result.totalLatency<<" ms\n";
    std::cout<<"Hops              : "<<result.hops<<'\n';
    std::cout<<"Reason            : "<<result.reason<<'\n';
    std::cout<<"Path              : ";
    if(result.path.empty())std::cout<<"NONE";
    for(size_t i=0;i<result.path.size();++i){
        std::cout<<result.path[i]->getName();
        if(i+1<result.path.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
}
IDS& NetworkSimulator::getIDS(){return ids;}
IDSTransmissionResult NetworkSimulator::sendWithIDS(NetworkNode* source,NetworkNode* destination,const std::string& protocol,int sourcePort,int destinationPort,int size){
    size_t previousAlerts=ids.getEvents().size();
    FirewallDecision decision=firewall.evaluate(source->getIPAddress(),destination->getIPAddress(),protocol,sourcePort,destinationPort);
    metrics.recordFirewall(decision.allowed);
    IDSTransmissionResult result{};
    result.firewallAllowed=decision.allowed;
    result.protocol=protocol;
    result.alertCount=0;
    result.anomalyDetected=false;
    result.totalLatency=0;
    result.hops=0;
    result.path.clear();
    if(!decision.allowed){
        metrics.recordPacket(size,false,true,false,0);
        metrics.recordProtocolPacket(protocol,false,true);
        if(!firewall.getLogs().empty())ids.analyzeFirewallLog(firewall.getLogs().back());
        result.delivered=false;
        result.alertCount=static_cast<int>(ids.getEvents().size()-previousAlerts);
        result.anomalyDetected=result.alertCount>0;
        result.reason="Firewall denied traffic and IDS analyzed the blocked connection.";
    }else{
        PacketForwardingResult forwarding=forwardPacket(source,destination,size,protocol);
        ids.analyzeTraffic(source->getIPAddress(),destination->getIPAddress(),protocol,sourcePort,destinationPort);
        result.delivered=forwarding.delivered;
        result.totalLatency=forwarding.totalLatency;
        result.hops=forwarding.hops;
        result.path=forwarding.path;
        result.alertCount=static_cast<int>(ids.getEvents().size()-previousAlerts);
        result.anomalyDetected=result.alertCount>0;
        result.reason=result.anomalyDetected?"Traffic delivered but IDS detected anomalous behavior.":forwarding.reason;
    }
    if(result.alertCount>0){
        int critical=0,high=0,medium=0,low=0;
        const auto& events=ids.getEvents();
        size_t begin=events.size()-static_cast<size_t>(result.alertCount);
        for(size_t i=begin;i<events.size();++i){
            if(events[i].severity==AlertSeverity::CRITICAL)++critical;
            else if(events[i].severity==AlertSeverity::HIGH)++high;
            else if(events[i].severity==AlertSeverity::MEDIUM)++medium;
            else ++low;
        }
        metrics.recordIDS(critical,high,medium,low);
    }
    return result;
}
void NetworkSimulator::displayIDSResult(const IDSTransmissionResult& result) const{
    std::cout<<"\n===== IDS TRANSMISSION =====\n";
    std::cout<<"Protocol          : "<<result.protocol<<'\n';
    std::cout<<"Firewall Decision : "<<(result.firewallAllowed?"ALLOW":"DENY")<<'\n';
    std::cout<<"Packet Delivered  : "<<(result.delivered?"YES":"NO")<<'\n';
    std::cout<<"Anomaly Detected  : "<<(result.anomalyDetected?"YES":"NO")<<'\n';
    std::cout<<"New IDS Alerts    : "<<result.alertCount<<'\n';
    std::cout<<"Total Latency     : "<<result.totalLatency<<" ms\n";
    std::cout<<"Hops              : "<<result.hops<<'\n';
    std::cout<<"Reason            : "<<result.reason<<'\n';
    std::cout<<"Path              : ";
    if(result.path.empty())std::cout<<"NONE";
    for(size_t i=0;i<result.path.size();++i){
        std::cout<<result.path[i]->getName();
        if(i+1<result.path.size())std::cout<<" -> ";
    }
    std::cout<<'\n';
}
