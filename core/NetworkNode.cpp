#include "NetworkNode.h"
#include "NetworkLink.h"
#include <iostream>

NetworkNode::NetworkNode(int id, const std::string& name, NodeType type)
    : id(id), name(name), type(type) {}

NetworkNode::~NetworkNode() {}

int NetworkNode::getId() const {
    return id;
}

std::string NetworkNode::getName() const {
    return name;
}

NodeType NetworkNode::getType() const {
    return type;
}

void NetworkNode::setIPAddress(const IPv4Address& ip) {
    ipAddress = ip;
}

IPv4Address NetworkNode::getIPAddress() const {
    return ipAddress;
}

void NetworkNode::addLink(NetworkLink* link) {
    links.push_back(link);
}

const std::vector<NetworkLink*>& NetworkNode::getLinks() const {
    return links;
}

void NetworkNode::display() const {
    std::cout << "ID: " << id
              << " | Name: " << name
              << " | IP: " << ipAddress.toString()
              << " | Links: " << links.size() << '\n';
}