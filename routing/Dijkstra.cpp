#include "Dijkstra.h"
#include <limits>
#include <queue>
#include <algorithm>
PathResult Dijkstra::calculate(NetworkNode* source,NetworkNode* destination,const std::vector<NetworkLink*>& links){
    distances.clear();
    previous.clear();
    using Pair=std::pair<double,NetworkNode*>;
    std::priority_queue<Pair,std::vector<Pair>,std::greater<Pair>> queue;
    std::vector<NetworkNode*> nodes;
    if(source==nullptr||destination==nullptr)return {{},std::numeric_limits<double>::infinity()};
    nodes.push_back(source);
    if(std::find(nodes.begin(),nodes.end(),destination)==nodes.end())nodes.push_back(destination);
    for(NetworkLink* link:links){
        if(link==nullptr||!link->isActive())continue;
        NetworkNode* a=link->getNodeA();
        NetworkNode* b=link->getNodeB();
        if(std::find(nodes.begin(),nodes.end(),a)==nodes.end())nodes.push_back(a);
        if(std::find(nodes.begin(),nodes.end(),b)==nodes.end())nodes.push_back(b);
    }
    for(NetworkNode* node:nodes){distances[node]=std::numeric_limits<double>::infinity();previous[node]=nullptr;}
    distances[source]=0.0;
    queue.push({0.0,source});
    while(!queue.empty()){
        auto current=queue.top();
        queue.pop();
        double currentDistance=current.first;
        NetworkNode* currentNode=current.second;
        if(currentDistance>distances[currentNode])continue;
        if(currentNode==destination)break;
        for(NetworkLink* link:links){
            if(link==nullptr||!link->isActive())continue;
            NetworkNode* neighbor=nullptr;
            if(link->getNodeA()==currentNode)neighbor=link->getNodeB();
            else if(link->getNodeB()==currentNode)neighbor=link->getNodeA();
            else continue;
            double newDistance=currentDistance+link->getLatency();
            if(newDistance<distances[neighbor]){
                distances[neighbor]=newDistance;
                previous[neighbor]=currentNode;
                queue.push({newDistance,neighbor});
            }
        }
    }
    PathResult result;
    result.cost=distances[destination];
    if(distances[destination]==std::numeric_limits<double>::infinity())return result;
    NetworkNode* current=destination;
    while(current!=nullptr){
        result.path.push_back(current);
        if(current==source)break;
        current=previous[current];
    }
    std::reverse(result.path.begin(),result.path.end());
    return result;
}
std::map<NetworkNode*,double> Dijkstra::getDistances() const{return distances;}