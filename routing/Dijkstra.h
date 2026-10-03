#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../core/NetworkNode.h"
#include "../core/NetworkLink.h"
#include "../core/Router.h"
#include <map>
#include <vector>

struct PathResult {
    std::vector<NetworkNode*> path;
    double cost;
};

class Dijkstra {
private:
    std::map<NetworkNode*, double> distances;
    std::map<NetworkNode*, NetworkNode*> previous;

public:
    PathResult calculate(
        NetworkNode* source,
        NetworkNode* destination,
        const std::vector<NetworkLink*>& links
    );

    std::map<NetworkNode*, double> getDistances() const;
};

#endif