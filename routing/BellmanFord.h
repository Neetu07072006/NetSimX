#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "../core/NetworkNode.h"
#include "../core/NetworkLink.h"
#include "DistanceVectorTable.h"
#include <map>
#include <vector>

struct BellmanFordResult {
    DistanceVectorTable table;
    int iterations;
    bool converged;
};

class BellmanFord {
private:
    std::map<NetworkNode*,double> distances;
    std::map<NetworkNode*,NetworkNode*> previous;
    std::map<NetworkNode*,NetworkNode*> nextHops;

public:
    BellmanFordResult calculate(
        NetworkNode* source,
        const std::vector<NetworkLink*>& links
    );

    std::map<NetworkNode*,double>
    getDistances() const;

    std::map<NetworkNode*,NetworkNode*>
    getNextHops() const;
};

#endif