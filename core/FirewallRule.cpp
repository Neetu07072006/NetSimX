#include "FirewallRule.h"
#include <iostream>
#include <algorithm>
FirewallRule::FirewallRule(int priority,FirewallAction action,const IPv4Address& sourceIP,const IPv4Address& destinationIP,const std::string& protocol,int sourcePort,int destinationPort,const std::string& description):priority(priority),action(action),sourceIP(sourceIP),destinationIP(destinationIP),protocol(protocol),sourcePort(sourcePort),destinationPort(destinationPort),description(description){}
int FirewallRule::getPriority() const{return priority;}
FirewallAction FirewallRule::getAction() const{return action;}
IPv4Address FirewallRule::getSourceIP() const{return sourceIP;}
IPv4Address FirewallRule::getDestinationIP() const{return destinationIP;}
std::string FirewallRule::getProtocol() const{return protocol;}
int FirewallRule::getSourcePort() const{return sourcePort;}
int FirewallRule::getDestinationPort() const{return destinationPort;}
std::string FirewallRule::getDescription() const{return description;}
bool FirewallRule::matches(const IPv4Address& source,const IPv4Address& destination,const std::string& protocol,int sourcePort,int destinationPort) const{
    bool sourceMatch=!sourceIP.isValid()||sourceIP==source;
    bool destinationMatch=!destinationIP.isValid()||destinationIP==destination;
    bool protocolMatch=protocol=="ANY"||this->protocol=="ANY"||this->protocol==protocol;
    bool sourcePortMatch=this->sourcePort==-1||this->sourcePort==sourcePort;
    bool destinationPortMatch=this->destinationPort==-1||this->destinationPort==destinationPort;
    return sourceMatch&&destinationMatch&&protocolMatch&&sourcePortMatch&&destinationPortMatch;
}
void FirewallRule::display() const{
    std::cout<<priority<<"\t"<<(action==FirewallAction::ALLOW?"ALLOW":"DENY")<<"\t";
    std::cout<<(sourceIP.isValid()?sourceIP.toString():"ANY")<<"\t";
    std::cout<<(destinationIP.isValid()?destinationIP.toString():"ANY")<<"\t";
    std::cout<<protocol<<"\t";
    std::cout<<(sourcePort==-1?"ANY":std::to_string(sourcePort))<<"\t";
    std::cout<<(destinationPort==-1?"ANY":std::to_string(destinationPort))<<"\t";
    std::cout<<description<<'\n';
}