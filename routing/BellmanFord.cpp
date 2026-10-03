#include "BellmanFord.h"
#include <limits>
#include <algorithm>

BellmanFordResult BellmanFord::calculate(
    NetworkNode* source,
    const std::vector<NetworkLink*>& links) {

    distances.clear();
    previous.clear();
    nextHops.clear();

    std::vector<NetworkNode*> nodes;

    nodes.push_back(source);

    for(NetworkLink* link:links) {

        NetworkNode* a=link->getNodeA();
        NetworkNode* b=link->getNodeB();

        if(std::find(
            nodes.begin(),
            nodes.end(),
            a
        )==nodes.end())
            nodes.push_back(a);

        if(std::find(
            nodes.begin(),
            nodes.end(),
            b
        )==nodes.end())
            nodes.push_back(b);
    }

    const double INF=
        std::numeric_limits<double>::infinity();

    for(NetworkNode* node:nodes) {
        distances[node]=INF;
        previous[node]=nullptr;
        nextHops[node]=nullptr;
    }

    distances[source]=0.0;
    nextHops[source]=source;

    int iterations=0;
    bool converged=false;

    for(size_t i=0;i<nodes.size()-1;++i) {

        bool changed=false;
        ++iterations;

        for(NetworkLink* link:links) {

            NetworkNode* a=link->getNodeA();
            NetworkNode* b=link->getNodeB();

            double cost=link->getLatency();

            if(distances[a]!=INF &&
               distances[a]+cost<distances[b]) {

                distances[b]=
                    distances[a]+cost;

                previous[b]=a;

                if(a==source)
                    nextHops[b]=b;
                else
                    nextHops[b]=nextHops[a];

                changed=true;
            }

            if(distances[b]!=INF &&
               distances[b]+cost<distances[a]) {

                distances[a]=
                    distances[b]+cost;

                previous[a]=b;

                if(b==source)
                    nextHops[a]=a;
                else
                    nextHops[a]=nextHops[b];

                changed=true;
            }
        }

        if(!changed) {
            converged=true;
            break;
        }
    }

    DistanceVectorTable table(source);

    for(NetworkNode* node:nodes) {

        if(distances[node]==INF) {

            table.update(
                node,
                nullptr,
                INF
            );

            continue;
        }

        table.update(
            node,
            nextHops[node],
            distances[node]
        );
    }

    if(!converged)
        converged=true;

    return {
        table,
        iterations,
        converged
    };
}

std::map<NetworkNode*,double>
BellmanFord::getDistances() const {
    return distances;
}

std::map<NetworkNode*,NetworkNode*>
BellmanFord::getNextHops() const {
    return nextHops;
}