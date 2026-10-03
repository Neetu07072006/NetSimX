#include "Router.h"
#include <iostream>
Router::Router(int id,const std::string& name):NetworkNode(id,name,NodeType::ROUTER),queueManager(10){}
void Router::addRoute(const IPv4Address& network,int prefixLength,const IPv4Address& nextHop,const std::string& interfaceName,int metric){routingTable.addRoute(Route(network,prefixLength,nextHop,interfaceName,metric));queueManager.createQueue(interfaceName);}
void Router::addDirectRoute(const IPv4Address& network,int prefixLength,const std::string& interfaceName,int metric){routingTable.addRoute(Route(network,prefixLength,IPv4Address(),interfaceName,metric));queueManager.createQueue(interfaceName);}
void Router::clearRoutes(){routingTable.clear();queueManager.clear();}
const Route* Router::lookupRoute(const IPv4Address& destination) const{return routingTable.lookup(destination);}
RoutingTable& Router::getRoutingTable(){return routingTable;}
RouterQueueManager& Router::getQueueManager(){return queueManager;}
void Router::createQueue(const std::string& interfaceName,size_t capacity){queueManager.createQueue(interfaceName,capacity);}
bool Router::enqueuePacket(const std::string& interfaceName,const QueuedPacket& packet){return queueManager.getQueue(interfaceName).enqueue(packet);}
bool Router::dequeuePacket(const std::string& interfaceName,QueuedPacket& packet){return queueManager.getQueue(interfaceName).dequeue(packet);}
bool Router::canReach(const IPv4Address& destination) const{return routingTable.lookup(destination)!=nullptr;}
void Router::displayRoutingTable() const{routingTable.display();}
void Router::displayQueues() const{queueManager.display();}
void Router::display() const{std::cout<<"Router | ID: "<<id<<" | Name: "<<name<<" | IP: "<<ipAddress.toString()<<" | Links: "<<links.size()<<'\n';}