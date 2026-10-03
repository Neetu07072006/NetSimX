#ifndef FIREWALL_H
#define FIREWALL_H
#include "FirewallRule.h"
#include <vector>
#include <string>
struct FirewallDecision{
    bool allowed;
    bool matched;
    int priority;
    std::string reason;
};
struct FirewallLog{
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    std::string protocol;
    int sourcePort;
    int destinationPort;
    bool allowed;
    int priority;
    std::string reason;
};
class Firewall{
private:
    std::vector<FirewallRule> rules;
    std::vector<FirewallLog> logs;
    bool defaultAllow;
public:
    Firewall(bool defaultAllow=true);
    void addRule(const FirewallRule& rule);
    void removeRule(int priority);
    void clearRules();
    void setDefaultAllow(bool value);
    bool getDefaultAllow() const;
    FirewallDecision evaluate(const IPv4Address& source,const IPv4Address& destination,const std::string& protocol,int sourcePort=-1,int destinationPort=-1);
    void clearLogs();
    const std::vector<FirewallRule>& getRules() const;
    const std::vector<FirewallLog>& getLogs() const;
    void displayRules() const;
    void displayLogs() const;
};
#endif