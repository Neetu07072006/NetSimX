#ifndef IDS_H
#define IDS_H
#include "IPv4Address.h"
#include "Firewall.h"
#include <vector>
#include <string>
#include <map>
enum class AlertSeverity{LOW,MEDIUM,HIGH,CRITICAL};
enum class AlertType{REPEATED_DENIAL,PORT_SCAN,TRAFFIC_SPIKE,SUSPICIOUS_PROTOCOL,MULTIPLE_ATTACKS};
struct IDSEvent{
    int id;
    AlertType type;
    AlertSeverity severity;
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    std::string protocol;
    int sourcePort;
    int destinationPort;
    int count;
    std::string description;
};
class IDS{
private:
    std::vector<IDSEvent> events;
    std::map<unsigned int,int> deniedBySource;
    std::map<unsigned int,std::vector<int>> portsBySource;
    std::map<unsigned int,int> trafficBySource;
    int nextEventId;
    int denialThreshold;
    int portScanThreshold;
    int trafficThreshold;
    std::string severityToString(AlertSeverity severity) const;
    std::string alertTypeToString(AlertType type) const;
public:
    IDS(int denialThreshold=5,int portScanThreshold=5,int trafficThreshold=20);
    void analyzeFirewallLog(const FirewallLog& log);
    void analyzeTraffic(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort);
    void detectPortScan(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort);
    void detectSuspiciousProtocol(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort);
    void clearEvents();
    void clearStatistics();
    int getEventCount() const;
    int getCriticalCount() const;
    int getHighCount() const;
    int getMediumCount() const;
    int getLowCount() const;
    const std::vector<IDSEvent>& getEvents() const;
    void displayEvents() const;
    void displayStatistics() const;
};
#endif