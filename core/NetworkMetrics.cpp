#include "NetworkMetrics.h"
#include <iostream>
#include <iomanip>
NetworkMetrics::NetworkMetrics(){reset();}
void NetworkMetrics::reset(){
totalPackets=0;
deliveredPackets=0;
droppedPackets=0;
expiredPackets=0;
totalBytes=0;
totalLatency=0;
firewallAllowed=0;
firewallDenied=0;
idsCritical=0;
idsHigh=0;
idsMedium=0;
idsLow=0;
queueDrops=0;
linkFailures=0;
recoveryEvents=0;
protocolPackets.clear();
protocolDelivered.clear();
protocolDropped.clear();
totalQueueDelay=0;
maximumQueueSize=0;
queueUtilization=0;
}
void NetworkMetrics::recordPacket(long long bytes,bool delivered,bool dropped,bool expired,double latency){
++totalPackets;
if(delivered)++deliveredPackets;
if(dropped)++droppedPackets;
if(expired)++expiredPackets;
if(bytes>0)totalBytes+=bytes;
if(latency>0)totalLatency+=latency;
}
void NetworkMetrics::recordProtocolPacket(const std::string& protocol,bool delivered,bool dropped){
++protocolPackets[protocol];
if(delivered)++protocolDelivered[protocol];
if(dropped)++protocolDropped[protocol];
}
void NetworkMetrics::recordQueue(long long drops,double totalDelay,double maximumSize,double utilization){
if(drops>0)queueDrops+=drops;
if(totalDelay>0)totalQueueDelay+=totalDelay;
if(maximumSize>maximumQueueSize)maximumQueueSize=maximumSize;
if(utilization>queueUtilization)queueUtilization=utilization;
}
void NetworkMetrics::recordFirewall(bool allowed){
if(allowed)++firewallAllowed;
else ++firewallDenied;
}
void NetworkMetrics::recordIDS(int critical,int high,int medium,int low){
idsCritical+=critical;
idsHigh+=high;
idsMedium+=medium;
idsLow+=low;
}
void NetworkMetrics::recordQueueDrop(long long count){
if(count>0)queueDrops+=count;
}
void NetworkMetrics::recordLinkFailure(){++linkFailures;}
void NetworkMetrics::recordRecovery(){++recoveryEvents;}
long long NetworkMetrics::getTotalPackets() const{return totalPackets;}
long long NetworkMetrics::getDeliveredPackets() const{return deliveredPackets;}
long long NetworkMetrics::getDroppedPackets() const{return droppedPackets;}
long long NetworkMetrics::getExpiredPackets() const{return expiredPackets;}
long long NetworkMetrics::getTotalBytes() const{return totalBytes;}
double NetworkMetrics::getTotalLatency() const{return totalLatency;}
double NetworkMetrics::getAverageLatency() const{return deliveredPackets==0?0.0:totalLatency/deliveredPackets;}
double NetworkMetrics::getThroughput() const{return totalLatency<=0?0.0:(static_cast<double>(totalBytes)*8.0)/(totalLatency/1000.0)/1000000.0;}
double NetworkMetrics::getPacketLossRate() const{return totalPackets==0?0.0:(static_cast<double>(droppedPackets)/totalPackets)*100.0;}
long long NetworkMetrics::getFirewallAllowed() const{return firewallAllowed;}
long long NetworkMetrics::getFirewallDenied() const{return firewallDenied;}
long long NetworkMetrics::getIDSAlerts() const{return idsCritical+idsHigh+idsMedium+idsLow;}
long long NetworkMetrics::getIDSCritical() const{return idsCritical;}
long long NetworkMetrics::getIDSHigh() const{return idsHigh;}
long long NetworkMetrics::getIDSMedium() const{return idsMedium;}
long long NetworkMetrics::getIDSLow() const{return idsLow;}
long long NetworkMetrics::getQueueDrops() const{return queueDrops;}
long long NetworkMetrics::getRecoveryEvents() const{return recoveryEvents;}
long long NetworkMetrics::getLinkFailures() const{return linkFailures;}
double NetworkMetrics::getTotalQueueDelay() const{return totalQueueDelay;}
double NetworkMetrics::getAverageQueueDelay() const{return queueDrops==0?0.0:totalQueueDelay/(queueDrops+1);}
double NetworkMetrics::getMaximumQueueSize() const{return maximumQueueSize;}
double NetworkMetrics::getQueueUtilization() const{return queueUtilization;}
long long NetworkMetrics::getProtocolPackets(const std::string& protocol) const{
auto it=protocolPackets.find(protocol);
return it==protocolPackets.end()?0:it->second;
}
long long NetworkMetrics::getProtocolDelivered(const std::string& protocol) const{
auto it=protocolDelivered.find(protocol);
return it==protocolDelivered.end()?0:it->second;
}
long long NetworkMetrics::getProtocolDropped(const std::string& protocol) const{
auto it=protocolDropped.find(protocol);
return it==protocolDropped.end()?0:it->second;
}
std::string NetworkMetrics::getNetworkHealth() const{
if(totalPackets==0)return "NO TRAFFIC";
if(getPacketLossRate()>=50.0||idsCritical>0)return "CRITICAL";
if(getPacketLossRate()>=20.0||idsHigh>=3||linkFailures>recoveryEvents)return "DEGRADED";
if(getPacketLossRate()>5.0||idsHigh>0||idsMedium>=5)return "WARNING";
return "HEALTHY";
}
void NetworkMetrics::display() const{
std::cout<<"\n===== NETWORK MONITORING & METRICS =====\n";
std::cout<<std::fixed<<std::setprecision(2);
std::cout<<"Network Health      : "<<getNetworkHealth()<<'\n';
std::cout<<"Total Packets       : "<<totalPackets<<'\n';
std::cout<<"Delivered Packets   : "<<deliveredPackets<<'\n';
std::cout<<"Dropped Packets     : "<<droppedPackets<<'\n';
std::cout<<"Expired Packets     : "<<expiredPackets<<'\n';
std::cout<<"Total Bytes         : "<<totalBytes<<'\n';
std::cout<<"Total Latency       : "<<totalLatency<<" ms\n";
std::cout<<"Average Latency     : "<<getAverageLatency()<<" ms\n";
std::cout<<"Throughput          : "<<getThroughput()<<" Mbps\n";
std::cout<<"Packet Loss Rate    : "<<getPacketLossRate()<<" %\n";
std::cout<<"Firewall Allowed    : "<<firewallAllowed<<'\n';
std::cout<<"Firewall Denied     : "<<firewallDenied<<'\n';
std::cout<<"IDS Alerts          : "<<getIDSAlerts()<<'\n';
std::cout<<"IDS Critical        : "<<idsCritical<<'\n';
std::cout<<"IDS High            : "<<idsHigh<<'\n';
std::cout<<"IDS Medium          : "<<idsMedium<<'\n';
std::cout<<"IDS Low             : "<<idsLow<<'\n';
std::cout<<"Queue Drops         : "<<queueDrops<<'\n';
std::cout<<"Queue Delay         : "<<getTotalQueueDelay()<<" ms\n";
std::cout<<"Queue Utilization   : "<<getQueueUtilization()<<" %\n";
std::cout<<"Maximum Queue Size  : "<<getMaximumQueueSize()<<'\n';
std::cout<<"Link Failures       : "<<linkFailures<<'\n';
std::cout<<"Recovery Events     : "<<recoveryEvents<<'\n';
}