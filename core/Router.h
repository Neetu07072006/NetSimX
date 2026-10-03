#ifndef ROUTER_H
#define ROUTER_H
#include "NetworkNode.h"
#include "RoutingTable.h"
#include "RouterQueueManager.h"
class Router:public NetworkNode{
private:
    RoutingTable routingTable;
    RouterQueueManager queueManager;
public:
    Router(int id,const std::string& name);
    void addRoute(const IPv4Address& network,int prefixLength,const IPv4Address& nextHop,const std::string& interfaceName,int metric=1);
    void addDirectRoute(const IPv4Address& network,int prefixLength,const std::string& interfaceName,int metric=0);
    void clearRoutes();
    const Route* lookupRoute(const IPv4Address& destination) const;
    RoutingTable& getRoutingTable();
    RouterQueueManager& getQueueManager();
    void createQueue(const std::string& interfaceName,size_t capacity=10);
    bool enqueuePacket(const std::string& interfaceName,const QueuedPacket& packet);
    bool dequeuePacket(const std::string& interfaceName,QueuedPacket& packet);
    bool canReach(const IPv4Address& destination) const;
    void displayRoutingTable() const;
    void displayQueues() const;
    void display() const override;
};
#endif