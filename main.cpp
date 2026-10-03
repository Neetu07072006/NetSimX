#include "simulation/NetworkSimulator.h"
#include <iostream>
#include <fstream>
#include <string>
std::string jsonEscape(const std::string& s){
std::string r;
for(char c:s){
if(c=='"')r+="\\\"";
else if(c=='\\')r+="\\\\";
else if(c=='\n')r+="\\n";
else if(c=='\r')r+="\\r";
else r+=c;
}
return r;
}
std::string alertTypeString(AlertType t){
switch(t){
case AlertType::REPEATED_DENIAL:return "REPEATED DENIAL";
case AlertType::PORT_SCAN:return "PORT SCAN";
case AlertType::TRAFFIC_SPIKE:return "TRAFFIC SPIKE";
case AlertType::SUSPICIOUS_PROTOCOL:return "SUSPICIOUS PROTOCOL";
case AlertType::MULTIPLE_ATTACKS:return "MULTIPLE ATTACKS";
}
return "UNKNOWN";
}
std::string severityString(AlertSeverity s){
switch(s){
case AlertSeverity::LOW:return "LOW";
case AlertSeverity::MEDIUM:return "MEDIUM";
case AlertSeverity::HIGH:return "HIGH";
case AlertSeverity::CRITICAL:return "CRITICAL";
}
return "UNKNOWN";
}
void exportSnapshot(NetworkSimulator& network){
NetworkMetrics& m=network.getMetrics();
IDS& ids=network.getIDS();
std::ofstream out("netsim_snapshot.json");
out<<"{\n";
out<<"\"health\":\""<<m.getNetworkHealth()<<"\",\n";
out<<"\"packets\":{";
out<<"\"total\":"<<m.getTotalPackets()<<",";
out<<"\"delivered\":"<<m.getDeliveredPackets()<<",";
out<<"\"dropped\":"<<m.getDroppedPackets()<<",";
out<<"\"expired\":"<<m.getExpiredPackets()<<",";
out<<"\"bytes\":"<<m.getTotalBytes()<<"},\n";
out<<"\"latency\":{";
out<<"\"total\":"<<m.getTotalLatency()<<",";
out<<"\"average\":"<<m.getAverageLatency()<<",";
out<<"\"throughput\":"<<m.getThroughput()<<"},\n";
out<<"\"security\":{";
out<<"\"firewallAllowed\":"<<m.getFirewallAllowed()<<",";
out<<"\"firewallDenied\":"<<m.getFirewallDenied()<<",";
out<<"\"idsAlerts\":"<<m.getIDSAlerts()<<",";
out<<"\"critical\":"<<m.getIDSCritical()<<",";
out<<"\"high\":"<<m.getIDSHigh()<<",";
out<<"\"medium\":"<<m.getIDSMedium()<<",";
out<<"\"low\":"<<m.getIDSLow()<<"},\n";
out<<"\"queue\":{";
out<<"\"drops\":"<<m.getQueueDrops()<<",";
out<<"\"utilization\":"<<m.getQueueUtilization()<<",";
out<<"\"maximum\":"<<m.getMaximumQueueSize()<<",";
out<<"\"averageDelay\":"<<m.getAverageQueueDelay()<<"},\n";
out<<"\"failures\":{";
out<<"\"linkFailures\":"<<m.getLinkFailures()<<",";
out<<"\"recoveries\":"<<m.getRecoveryEvents()<<"},\n";
out<<"\"nodes\":[";
out<<"{\"id\":\"pc1\",\"name\":\"PC1\",\"type\":\"HOST\",\"ip\":\"192.168.1.10\",\"x\":90,\"y\":210},";
out<<"{\"id\":\"r1\",\"name\":\"R1\",\"type\":\"ROUTER\",\"ip\":\"10.0.1.1\",\"x\":270,\"y\":110},";
out<<"{\"id\":\"r2\",\"name\":\"R2\",\"type\":\"ROUTER\",\"ip\":\"10.0.2.1\",\"x\":500,\"y\":80},";
out<<"{\"id\":\"r3\",\"name\":\"R3\",\"type\":\"ROUTER\",\"ip\":\"10.0.3.1\",\"x\":500,\"y\":240},";
out<<"{\"id\":\"r4\",\"name\":\"R4\",\"type\":\"ROUTER\",\"ip\":\"10.0.4.1\",\"x\":730,\"y\":160},";
out<<"{\"id\":\"pc4\",\"name\":\"PC4\",\"type\":\"HOST\",\"ip\":\"192.168.4.10\",\"x\":910,\"y\":160}],\n";
out<<"\"links\":[";
out<<"{\"a\":\"pc1\",\"b\":\"r1\",\"latency\":1,\"status\":\"ACTIVE\"},";
out<<"{\"a\":\"r1\",\"b\":\"r2\",\"latency\":5,\"status\":\"ACTIVE\"},";
out<<"{\"a\":\"r1\",\"b\":\"r3\",\"latency\":2,\"status\":\"ACTIVE\"},";
out<<"{\"a\":\"r2\",\"b\":\"r4\",\"latency\":2,\"status\":\"ACTIVE\"},";
out<<"{\"a\":\"r3\",\"b\":\"r4\",\"latency\":1,\"status\":\"ACTIVE\"},";
out<<"{\"a\":\"r4\",\"b\":\"pc4\",\"latency\":1,\"status\":\"ACTIVE\"}],\n";
out<<"\"routes\":[";
out<<"{\"source\":\"PC1\",\"destination\":\"PC4\",\"path\":\"PC1 → R1 → R3 → R4 → PC4\",\"cost\":\"5 ms\"},";
out<<"{\"source\":\"PC1\",\"destination\":\"R4\",\"path\":\"PC1 → R1 → R3 → R4\",\"cost\":\"4 ms\"},";
out<<"{\"source\":\"R1\",\"destination\":\"PC4\",\"path\":\"R1 → R3 → R4 → PC4\",\"cost\":\"4 ms\"}],\n";
out<<"\"traffic\":[";
out<<"{\"protocol\":\"TCP\",\"packets\":"<<m.getProtocolPackets("TCP")<<",\"delivered\":"<<m.getProtocolDelivered("TCP")<<",\"dropped\":"<<m.getProtocolDropped("TCP")<<"},";
out<<"{\"protocol\":\"UDP\",\"packets\":"<<m.getProtocolPackets("UDP")<<",\"delivered\":"<<m.getProtocolDelivered("UDP")<<",\"dropped\":"<<m.getProtocolDropped("UDP")<<"},";
out<<"{\"protocol\":\"ICMP\",\"packets\":"<<m.getProtocolPackets("ICMP")<<",\"delivered\":"<<m.getProtocolDelivered("ICMP")<<",\"dropped\":"<<m.getProtocolDropped("ICMP")<<"},";
out<<"{\"protocol\":\"TELNET\",\"packets\":"<<m.getProtocolPackets("TELNET")<<",\"delivered\":"<<m.getProtocolDelivered("TELNET")<<",\"dropped\":"<<m.getProtocolDropped("TELNET")<<"}],\n";
out<<"\"alerts\":[";
const auto& events=ids.getEvents();
for(size_t i=0;i<events.size();++i){
const auto& e=events[i];
out<<"{\"id\":"<<e.id<<",";
out<<"\"type\":\""<<alertTypeString(e.type)<<"\",";
out<<"\"severity\":\""<<severityString(e.severity)<<"\",";
out<<"\"source\":\""<<e.sourceIP.toString()<<"\",";
out<<"\"protocol\":\""<<jsonEscape(e.protocol)<<"\",";
out<<"\"port\":"<<e.destinationPort<<"}";
if(i+1<events.size())out<<",";
}
out<<"]\n";
out<<"}\n";
out.close();
}
void setupNetwork(NetworkSimulator& network,Router*& r1,Router*& r2,Router*& r3,Router*& r4,Host*& pc1,Host*& pc4){
r1=network.addRouter("Router-1");
r2=network.addRouter("Router-2");
r3=network.addRouter("Router-3");
r4=network.addRouter("Router-4");
pc1=network.addHost("PC-1","AA:BB:CC:DD:EE:01");
pc4=network.addHost("PC-4","AA:BB:CC:DD:EE:04");
r1->setIPAddress(IPv4Address("10.0.1.1"));
r2->setIPAddress(IPv4Address("10.0.2.1"));
r3->setIPAddress(IPv4Address("10.0.3.1"));
r4->setIPAddress(IPv4Address("10.0.4.1"));
pc1->setIPAddress(IPv4Address("192.168.1.10"));
pc4->setIPAddress(IPv4Address("192.168.4.10"));
network.connect(r1,r2,1000,5);
network.connect(r1,r3,1000,2);
network.connect(r2,r4,1000,2);
network.connect(r3,r4,1000,1);
network.connect(r1,pc1,1000,1);
network.connect(r4,pc4,1000,1);
}
void runTCP(NetworkSimulator& network,Host* pc1,Host* pc4){
std::cout<<"\n===== TCP TRAFFIC SCENARIO =====\n";
TCPSegment tcpSegment(5001,8080,1000,0,65535,static_cast<uint8_t>(TCPFlag::NONE),"Hello from PC-1 using TCP");
tcpSegment.display();
network.displayTCPResult(network.sendTCP(pc1,pc4,5001,8080,1000,0,65535,static_cast<uint8_t>(TCPFlag::NONE),"Hello from PC-1 using TCP"));
std::cout<<"\n===== TCP HANDSHAKE =====\n";
network.displayTCPHandshakeResult(network.establishTCPConnection(pc1,pc4,5001,8080,1000,2000,65535));
std::cout<<"\n===== RELIABLE TCP =====\n";
network.displayTCPReliableResult(network.sendReliableTCP(pc1,pc4,5001,8080,"Reliable TCP message",3000,65535,3));
std::cout<<"\n===== TCP TERMINATION =====\n";
network.displayTCPTerminationResult(network.terminateTCPConnection(pc1,pc4,5001,8080,4000,5000,65535));
std::cout<<"\n===== TCP CONGESTION CONTROL =====\n";
TCPCongestionControl congestion;
congestion.display();
network.displayTCPCongestionResult(network.simulateTCPCongestion(pc1,pc4,5001,8080,12,65535,0.0));
std::cout<<"\n===== TCP LOSS =====\n";
network.displayTCPCongestionResult(network.simulateTCPCongestion(pc1,pc4,5001,8080,12,65535,20.0));
}
void runUDP(NetworkSimulator& network,Host* pc1,Host* pc4){
std::cout<<"\n===== UDP TRAFFIC SCENARIO =====\n";
network.displayUDPResult(network.sendUDP(pc1,pc4,5000,8080,"Hello from PC-1 to PC-4 using UDP"));
network.displayUDPResult(network.sendUDP(pc1,pc4,6000,53,"DNS request from PC-1"));
}
void runICMP(NetworkSimulator& network,Host* pc1,Host* pc4){
std::cout<<"\n===== ICMP TRAFFIC SCENARIO =====\n";
network.displayForwardingResult(network.forwardPacket(pc1,pc4,1024,"ICMP"));
network.displayForwardingResult(network.forwardPacket(pc1,pc4,512,"ICMP"));
network.displayForwardingResult(network.forwardPacket(pc4,pc1,1024,"ICMP"));
}
void runFirewall(NetworkSimulator& network,Host* pc1,Host* pc4){
std::cout<<"\n===== FIREWALL SCENARIO =====\n";
Firewall& firewall=network.getFirewall();
firewall.clearRules();
firewall.setDefaultAllow(true);
firewall.addRule(FirewallRule(10,FirewallAction::DENY,IPv4Address("192.168.1.10"),IPv4Address("192.168.4.10"),"TCP",-1,8080,"Block TCP port 8080"));
firewall.addRule(FirewallRule(20,FirewallAction::ALLOW,IPv4Address("192.168.1.10"),IPv4Address("192.168.4.10"),"UDP",-1,53,"Allow DNS"));
firewall.addRule(FirewallRule(30,FirewallAction::ALLOW,IPv4Address(),IPv4Address(),"ICMP",-1,-1,"Allow ICMP"));
firewall.displayRules();
std::cout<<"\n===== FIREWALL TCP TEST =====\n";
network.displayFirewallResult(network.sendThroughFirewall(pc1,pc4,"TCP",5001,8080,1024));
std::cout<<"\n===== FIREWALL UDP TEST =====\n";
network.displayFirewallResult(network.sendThroughFirewall(pc1,pc4,"UDP",5000,53,512));
std::cout<<"\n===== FIREWALL ICMP TEST =====\n";
network.displayFirewallResult(network.sendThroughFirewall(pc1,pc4,"ICMP",-1,-1,1024));
std::cout<<"\n===== FIREWALL LOGS =====\n";
firewall.displayLogs();
}
void runIDS(NetworkSimulator& network,Host* pc1,Host* pc4){
std::cout<<"\n===== IDS SCENARIO =====\n";
IDS& ids=network.getIDS();
ids.clearEvents();
ids.clearStatistics();
std::cout<<"\n===== NORMAL ICMP TRAFFIC =====\n";
network.displayIDSResult(network.sendWithIDS(pc1,pc4,"ICMP",-1,-1,1024));
std::cout<<"\n===== PORT SCAN SIMULATION =====\n";
network.sendWithIDS(pc1,pc4,"TCP",5000,21,512);
network.sendWithIDS(pc1,pc4,"TCP",5000,22,512);
network.sendWithIDS(pc1,pc4,"TCP",5000,23,512);
network.sendWithIDS(pc1,pc4,"TCP",5000,25,512);
network.sendWithIDS(pc1,pc4,"TCP",5000,80,512);
std::cout<<"\n===== REPEATED DENIAL SIMULATION =====\n";
for(int i=0;i<10;++i)network.sendWithIDS(pc1,pc4,"TCP",5000,8080,512);
std::cout<<"\n===== SUSPICIOUS PROTOCOL =====\n";
network.sendWithIDS(pc1,pc4,"TELNET",4000,23,512);
ids.displayEvents();
ids.displayStatistics();
}
void runFailure(NetworkSimulator& network,Host* pc1,Host* pc4,Router* r1,Router* r3){
std::cout<<"\n===== FAILURE SCENARIO =====\n";
network.buildRoutingTables();
network.displayFailureSimulationResult(network.simulateFailure(pc1,pc4,r1,r3));
network.failLink(r1,r3);
network.buildRoutingTables();
std::cout<<"\n===== FORWARDING DURING FAILURE =====\n";
network.displayForwardingResult(network.forwardPacket(pc1,pc4,1024,"ICMP"));
network.recoverLink(r1,r3);
network.buildRoutingTables();
std::cout<<"\n===== FORWARDING AFTER RECOVERY =====\n";
network.displayForwardingResult(network.forwardPacket(pc1,pc4,1024,"ICMP"));
}
void runQueue(NetworkSimulator& network,Router* r1){
std::cout<<"\n===== QUEUE SCENARIO =====\n";
network.displayQueueSimulationResult(network.simulateNetworkQueue(r1,"GigabitEthernet0/0",15,1024,"TCP",5));
r1->displayQueues();
}
void runRouting(NetworkSimulator& network,Router* r1,Router* r2,Router* r3,Router* r4,Host* pc1,Host* pc4){
std::cout<<"\n===== ROUTING =====\n";
network.displayPath(r1,r4);
network.displayPath(r1,pc4);
network.displayPath(pc1,pc4);
network.buildRoutingTables();
r1->displayRoutingTable();
r2->displayRoutingTable();
r3->displayRoutingTable();
r4->displayRoutingTable();
network.displayDistanceVector(r1);
network.displayDistanceVector(r2);
network.displayDistanceVector(r3);
network.displayDistanceVector(r4);
network.buildDistanceVectorTables();
network.buildBellmanFordRoutingTables();
}
void runFull(NetworkSimulator& network,Router* r1,Router* r2,Router* r3,Router* r4,Host* pc1,Host* pc4){
network.displayTopology();
runRouting(network,r1,r2,r3,r4,pc1,pc4);
std::cout<<"\n===== PHASE 9: PACKET FORWARDING =====\n";
network.displayForwardingResult(network.forwardPacket(pc1,pc4,1024,"ICMP"));
std::cout<<"\n===== PHASE 10: UDP =====\n";
runUDP(network,pc1,pc4);
std::cout<<"\n===== PHASE 11: TCP =====\n";
runTCP(network,pc1,pc4);
std::cout<<"\n===== PHASE 14: NETWORK QUEUES =====\n";
runQueue(network,r1);
std::cout<<"\n===== PHASE 15: FAILURE SIMULATION =====\n";
runFailure(network,pc1,pc4,r1,r3);
runFirewall(network,pc1,pc4);
runIDS(network,pc1,pc4);
}
int main(int argc,char* argv[]){
std::string mode=argc>1?argv[1]:"full";
if(mode!="full"&&mode!="tcp"&&mode!="udp"&&mode!="icmp"&&mode!="firewall"&&mode!="ids"&&mode!="failure"&&mode!="queue"){
std::cout<<"Invalid simulation mode: "<<mode<<"\n";
std::cout<<"Available modes: full tcp udp icmp firewall ids failure queue\n";
return 1;
}
NetworkSimulator network;
Router* r1;
Router* r2;
Router* r3;
Router* r4;
Host* pc1;
Host* pc4;
setupNetwork(network,r1,r2,r3,r4,pc1,pc4);
network.resetMetrics();
if(mode=="full")runFull(network,r1,r2,r3,r4,pc1,pc4);
else if(mode=="tcp")runTCP(network,pc1,pc4);
else if(mode=="udp")runUDP(network,pc1,pc4);
else if(mode=="icmp")runICMP(network,pc1,pc4);
else if(mode=="firewall")runFirewall(network,pc1,pc4);
else if(mode=="ids")runIDS(network,pc1,pc4);
else if(mode=="failure")runFailure(network,pc1,pc4,r1,r3);
else if(mode=="queue")runQueue(network,r1);
network.displayMetrics();
exportSnapshot(network);
std::cout<<"\n===== SNAPSHOT EXPORTED =====\n";
std::cout<<"netsim_snapshot.json created successfully.\n";
return 0;
}
