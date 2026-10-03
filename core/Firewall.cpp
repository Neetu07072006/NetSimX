#include "Firewall.h"
#include <iostream>
#include <algorithm>
Firewall::Firewall(bool defaultAllow):defaultAllow(defaultAllow){}
void Firewall::addRule(const FirewallRule& rule){
    rules.push_back(rule);
    std::sort(rules.begin(),rules.end(),[](const FirewallRule& a,const FirewallRule& b){return a.getPriority()<b.getPriority();});
}
void Firewall::removeRule(int priority){
    rules.erase(std::remove_if(rules.begin(),rules.end(),[priority](const FirewallRule& rule){return rule.getPriority()==priority;}),rules.end());
}
void Firewall::clearRules(){rules.clear();}
void Firewall::setDefaultAllow(bool value){defaultAllow=value;}
bool Firewall::getDefaultAllow() const{return defaultAllow;}
FirewallDecision Firewall::evaluate(const IPv4Address& source,const IPv4Address& destination,const std::string& protocol,int sourcePort,int destinationPort){
    for(const FirewallRule& rule:rules){
        if(!rule.matches(source,destination,protocol,sourcePort,destinationPort))continue;
        bool allowed=rule.getAction()==FirewallAction::ALLOW;
        std::string reason=allowed?"Traffic allowed by firewall rule.":"Traffic denied by firewall rule.";
        logs.push_back({source,destination,protocol,sourcePort,destinationPort,allowed,rule.getPriority(),reason});
        return {allowed,true,rule.getPriority(),reason};
    }
    bool allowed=defaultAllow;
    std::string reason=allowed?"Traffic allowed by default firewall policy.":"Traffic denied by default firewall policy.";
    logs.push_back({source,destination,protocol,sourcePort,destinationPort,allowed,-1,reason});
    return {allowed,false,-1,reason};
}
void Firewall::clearLogs(){logs.clear();}
const std::vector<FirewallRule>& Firewall::getRules() const{return rules;}
const std::vector<FirewallLog>& Firewall::getLogs() const{return logs;}
void Firewall::displayRules() const{
    std::cout<<"\n===== FIREWALL RULES =====\n";
    if(rules.empty()){std::cout<<"No firewall rules configured.\n";return;}
    std::cout<<"Priority\tAction\tSource\tDestination\tProtocol\tSrcPort\tDstPort\tDescription\n";
    for(const FirewallRule& rule:rules)rule.display();
}
void Firewall::displayLogs() const{
    std::cout<<"\n===== FIREWALL LOGS =====\n";
    if(logs.empty()){std::cout<<"No firewall events recorded.\n";return;}
    std::cout<<"Source\tDestination\tProtocol\tSrcPort\tDstPort\tAction\tRule\tReason\n";
    for(const FirewallLog& log:logs){
        std::cout<<log.sourceIP.toString()<<'\t';
        std::cout<<log.destinationIP.toString()<<'\t';
        std::cout<<log.protocol<<'\t';
        std::cout<<log.sourcePort<<'\t';
        std::cout<<log.destinationPort<<'\t';
        std::cout<<(log.allowed?"ALLOW":"DENY")<<'\t';
        std::cout<<log.priority<<'\t';
        std::cout<<log.reason<<'\n';
    }
}