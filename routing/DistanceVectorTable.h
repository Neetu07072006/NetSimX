#ifndef DISTANCEVECTORTABLE_H
#define DISTANCEVECTORTABLE_H

#include "../core/NetworkNode.h"
#include <map>
#include <limits>

struct DistanceVectorEntry {
    NetworkNode* destination;
    NetworkNode* nextHop;
    double cost;
};

class DistanceVectorTable {
private:
    NetworkNode* owner;
    std::map<NetworkNode*,DistanceVectorEntry> entries;

public:
    DistanceVectorTable(NetworkNode* owner=nullptr);

    void setOwner(NetworkNode* owner);

    void update(
        NetworkNode* destination,
        NetworkNode* nextHop,
        double cost
    );

    bool contains(NetworkNode* destination) const;

    DistanceVectorEntry get(
        NetworkNode* destination
    ) const;

    double getCost(
        NetworkNode* destination
    ) const;

    NetworkNode* getNextHop(
        NetworkNode* destination
    ) const;

    const std::map<NetworkNode*,DistanceVectorEntry>&
    getEntries() const;

    void clear();

    void display() const;
};

#endif