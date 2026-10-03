#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include "Route.h"
#include <vector>

class RoutingTable {
private:
    std::vector<Route> routes;

public:
    void addRoute(const Route& route);
    void removeRoute(const IPv4Address& network,
                     int prefixLength);

    const Route* lookup(
        const IPv4Address& destination) const;

    void clear();

    const std::vector<Route>& getRoutes() const;

    void display() const;
};

#endif