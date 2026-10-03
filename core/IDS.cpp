#include "IDS.h"
#include <iostream>
#include <algorithm>
IDS::IDS(int denialThreshold,int portScanThreshold,int trafficThreshold):nextEventId(1),denialThreshold(denialThreshold),portScanThreshold(portScanThreshold),trafficThreshold(trafficThreshold){}
std::string IDS::severityToString(AlertSeverity severity) const{
    switch(severity){
        case AlertSeverity::LOW:return "LOW";
        case AlertSeverity::MEDIUM:return "MEDIUM";
        case AlertSeverity::HIGH:return "HIGH";
        case AlertSeverity::CRITICAL:return "CRITICAL";
    }
    return "UNKNOWN";
}
std::string IDS::alertTypeToString(AlertType type) const{
    switch(type){
        case AlertType::REPEATED_DENIAL:return "REPEATED DENIAL";
        case AlertType::PORT_SCAN:return "PORT SCAN";
        case AlertType::TRAFFIC_SPIKE:return "TRAFFIC SPIKE";
        case AlertType::SUSPICIOUS_PROTOCOL:return "SUSPICIOUS PROTOCOL";
        case AlertType::MULTIPLE_ATTACKS:return "MULTIPLE ATTACKS";
    }
    return "UNKNOWN";
}
void IDS::analyzeFirewallLog(const FirewallLog& log){
    unsigned int source=log.sourceIP.toInteger();
    if(!log.allowed){
        ++deniedBySource[source];
        if(deniedBySource[source]==denialThreshold){
            events.push_back({nextEventId++,AlertType::REPEATED_DENIAL,AlertSeverity::HIGH,log.sourceIP,log.destinationIP,log.protocol,log.sourcePort,log.destinationPort,deniedBySource[source],"Repeated firewall-denied connections detected from source."});
        }
        if(deniedBySource[source]>=denialThreshold*2){
            events.push_back({nextEventId++,AlertType::MULTIPLE_ATTACKS,AlertSeverity::CRITICAL,log.sourceIP,log.destinationIP,log.protocol,log.sourcePort,log.destinationPort,deniedBySource[source],"Multiple repeated denied connections indicate a possible coordinated attack."});
        }
    }
    analyzeTraffic(log.sourceIP,log.destinationIP,log.protocol,log.sourcePort,log.destinationPort);
}
void IDS::analyzeTraffic(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort){
    unsigned int source=sourceIP.toInteger();
    ++trafficBySource[source];
    if(trafficBySource[source]==trafficThreshold){
        events.push_back({nextEventId++,AlertType::TRAFFIC_SPIKE,AlertSeverity::MEDIUM,sourceIP,destinationIP,protocol,sourcePort,destinationPort,trafficBySource[source],"Abnormally high traffic volume detected from source."});
    }
    detectPortScan(sourceIP,destinationIP,protocol,sourcePort,destinationPort);
    detectSuspiciousProtocol(sourceIP,destinationIP,protocol,sourcePort,destinationPort);
}
void IDS::detectPortScan(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort){
    if(destinationPort<0)return;
    unsigned int source=sourceIP.toInteger();
    std::vector<int>& ports=portsBySource[source];
    if(std::find(ports.begin(),ports.end(),destinationPort)==ports.end())ports.push_back(destinationPort);
    if(static_cast<int>(ports.size())==portScanThreshold){
        events.push_back({nextEventId++,AlertType::PORT_SCAN,AlertSeverity::HIGH,sourceIP,destinationIP,protocol,sourcePort,destinationPort,static_cast<int>(ports.size()),"Multiple destination ports contacted by the same source."});
    }
}
void IDS::detectSuspiciousProtocol(const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort){
    if(protocol!="TCP"&&protocol!="UDP"&&protocol!="ICMP"){
        events.push_back({nextEventId++,AlertType::SUSPICIOUS_PROTOCOL,AlertSeverity::MEDIUM,sourceIP,destinationIP,protocol,sourcePort,destinationPort,1,"Unknown or unsupported network protocol detected."});
    }
}
void IDS::clearEvents(){events.clear();nextEventId=1;}
void IDS::clearStatistics(){deniedBySource.clear();portsBySource.clear();trafficBySource.clear();}
int IDS::getEventCount() const{return static_cast<int>(events.size());}
int IDS::getCriticalCount() const{
    int count=0;
    for(const auto& event:events)if(event.severity==AlertSeverity::CRITICAL)++count;
    return count;
}
int IDS::getHighCount() const{
    int count=0;
    for(const auto& event:events)if(event.severity==AlertSeverity::HIGH)++count;
    return count;
}
int IDS::getMediumCount() const{
    int count=0;
    for(const auto& event:events)if(event.severity==AlertSeverity::MEDIUM)++count;
    return count;
}
int IDS::getLowCount() const{
    int count=0;
    for(const auto& event:events)if(event.severity==AlertSeverity::LOW)++count;
    return count;
}
const std::vector<IDSEvent>& IDS::getEvents() const{return events;}
void IDS::displayEvents() const{
    std::cout<<"\n===== IDS ALERTS =====\n";
    if(events.empty()){std::cout<<"No security anomalies detected.\n";return;}
    std::cout<<"ID\tType\tSeverity\tSource\tDestination\tProtocol\tCount\tDescription\n";
    for(const auto& event:events){
        std::cout<<event.id<<'\t';
        std::cout<<alertTypeToString(event.type)<<'\t';
        std::cout<<severityToString(event.severity)<<'\t';
        std::cout<<event.sourceIP.toString()<<'\t';
        std::cout<<event.destinationIP.toString()<<'\t';
        std::cout<<event.protocol<<'\t';
        std::cout<<event.count<<'\t';
        std::cout<<event.description<<'\n';
    }
}
void IDS::displayStatistics() const{
    std::cout<<"\n===== IDS STATISTICS =====\n";
    std::cout<<"Total Alerts       : "<<getEventCount()<<'\n';
    std::cout<<"Critical Alerts    : "<<getCriticalCount()<<'\n';
    std::cout<<"High Alerts        : "<<getHighCount()<<'\n';
    std::cout<<"Medium Alerts      : "<<getMediumCount()<<'\n';
    std::cout<<"Low Alerts         : "<<getLowCount()<<'\n';
    std::cout<<"Monitored Sources  : "<<trafficBySource.size()<<'\n';
}