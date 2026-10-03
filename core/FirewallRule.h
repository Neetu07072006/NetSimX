#ifndef FIREWALLRULE_H
#define FIREWALLRULE_H
#include "IPv4Address.h"
#include <string>
enum class FirewallAction{ALLOW,DENY};
class FirewallRule{
private:
    int priority;
    FirewallAction action;
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    std::string protocol;
    int sourcePort;
    int destinationPort;
    std::string description;
public:
    FirewallRule(int priority,FirewallAction action,const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol="ANY",int sourcePort=-1,int destinationPort=-1,const std::string& description="");
    int getPriority() const;
    FirewallAction getAction() const;
    IPv4Address getSourceIP() const;
    IPv4Address getDestinationIP() const;
    std::string getProtocol() const;
    int getSourcePort() const;
    int getDestinationPort() const;
    std::string getDescription() const;
    bool matches(const IPv4Address& source,const IPv4Address& destination,const std::string& protocol,int sourcePort=-1,int destinationPort=-1) const;
    void display() const;
};
#endif