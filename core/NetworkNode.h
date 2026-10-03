#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include "IPv4Address.h"
#include <string>
#include <vector>

class NetworkLink;

enum class NodeType {
    HOST,
    ROUTER,
    SWITCH
};

class NetworkNode {
protected:
    int id;
    std::string name;
    NodeType type;
    IPv4Address ipAddress;
    std::vector<NetworkLink*> links;

public:
    NetworkNode(int id, const std::string& name, NodeType type);
    virtual ~NetworkNode();

    int getId() const;
    std::string getName() const;
    NodeType getType() const;

    void setIPAddress(const IPv4Address& ip);
    IPv4Address getIPAddress() const;

    void addLink(NetworkLink* link);
    const std::vector<NetworkLink*>& getLinks() const;

    virtual void display() const;
};

#endif