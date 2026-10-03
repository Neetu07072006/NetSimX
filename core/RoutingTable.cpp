#include "RoutingTable.h"
#include <iostream>

void RoutingTable::addRoute(
    const Route& route) {

    routes.push_back(route);
}

void RoutingTable::removeRoute(
    const IPv4Address& network,
    int prefixLength) {

    for (auto it = routes.begin();
         it != routes.end();
         ++it) {

        if (it->getNetwork() == network &&
            it->getPrefixLength() == prefixLength) {

            routes.erase(it);
            return;
        }
    }
}

const Route* RoutingTable::lookup(
    const IPv4Address& destination) const {

    const Route* bestRoute = nullptr;

    for (const auto& route : routes) {

        if (!route.matches(destination))
            continue;

        if (bestRoute == nullptr ||
            route.getPrefixLength() >
            bestRoute->getPrefixLength()) {

            bestRoute = &route;
        }
        else if (
            route.getPrefixLength() ==
            bestRoute->getPrefixLength() &&
            route.getMetric() <
            bestRoute->getMetric()) {

            bestRoute = &route;
        }
    }

    return bestRoute;
}

void RoutingTable::clear() {
    routes.clear();
}

const std::vector<Route>&
RoutingTable::getRoutes() const {
    return routes;
}

void RoutingTable::display() const {

    std::cout << "\n===== ROUTING TABLE =====\n";

    if (routes.empty()) {
        std::cout << "Routing table is empty.\n";
        return;
    }

    std::cout << "Network\t\tNext Hop\tInterface\tMetric\n";

    for (const auto& route : routes)
        route.display();
}