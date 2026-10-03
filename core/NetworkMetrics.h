#ifndef NETWORKMETRICS_H
#define NETWORKMETRICS_H
#include <string>
#include <map>
class NetworkMetrics{
private:
long long totalPackets;
long long deliveredPackets;
long long droppedPackets;
long long expiredPackets;
long long totalBytes;
double totalLatency;
long long firewallAllowed;
long long firewallDenied;
long long idsCritical;
long long idsHigh;
long long idsMedium;
long long idsLow;
long long queueDrops;
long long linkFailures;
long long recoveryEvents;
std::map<std::string,long long> protocolPackets;
std::map<std::string,long long> protocolDelivered;
std::map<std::string,long long> protocolDropped;
double totalQueueDelay;
double maximumQueueSize;
double queueUtilization;
public:
NetworkMetrics();
void reset();
void recordPacket(long long bytes,bool delivered,bool dropped,bool expired,double latency);
void recordProtocolPacket(const std::string& protocol,bool delivered,bool dropped);
void recordQueue(long long drops,double totalDelay,double maximumSize,double utilization);
void recordFirewall(bool allowed);
void recordIDS(int critical,int high,int medium,int low);
void recordQueueDrop(long long count=1);
void recordLinkFailure();
void recordRecovery();
long long getTotalPackets() const;
long long getDeliveredPackets() const;
long long getDroppedPackets() const;
long long getExpiredPackets() const;
long long getTotalBytes() const;
double getTotalLatency() const;
double getAverageLatency() const;
double getThroughput() const;
double getPacketLossRate() const;
long long getFirewallAllowed() const;
long long getFirewallDenied() const;
long long getIDSAlerts() const;
long long getIDSCritical() const;
long long getIDSHigh() const;
long long getIDSMedium() const;
long long getIDSLow() const;
long long getQueueDrops() const;
long long getRecoveryEvents() const;
long long getLinkFailures() const;
double getTotalQueueDelay() const;
double getAverageQueueDelay() const;
double getMaximumQueueSize() const;
double getQueueUtilization() const;
long long getProtocolPackets(const std::string& protocol) const;
long long getProtocolDelivered(const std::string& protocol) const;
long long getProtocolDropped(const std::string& protocol) const;
std::string getNetworkHealth() const;
void display() const;
};
#endif