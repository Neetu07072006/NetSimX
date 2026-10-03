#include "DistanceVectorTable.h"
#include <iostream>

DistanceVectorTable::DistanceVectorTable(
    NetworkNode* owner)
    : owner(owner) {}

void DistanceVectorTable::setOwner(
    NetworkNode* owner) {
    this->owner=owner;
}

void DistanceVectorTable::update(
    NetworkNode* destination,
    NetworkNode* nextHop,
    double cost) {

    entries[destination]={
        destination,
        nextHop,
        cost
    };
}

bool DistanceVectorTable::contains(
    NetworkNode* destination) const {

    return entries.find(destination)!=entries.end();
}

DistanceVectorEntry DistanceVectorTable::get(
    NetworkNode* destination) const {

    auto it=entries.find(destination);

    if(it==entries.end()) {
        return {
            destination,
            nullptr,
            std::numeric_limits<double>::infinity()
        };
    }

    return it->second;
}

double DistanceVectorTable::getCost(
    NetworkNode* destination) const {

    return get(destination).cost;
}

NetworkNode* DistanceVectorTable::getNextHop(
    NetworkNode* destination) const {

    return get(destination).nextHop;
}

const std::map<NetworkNode*,DistanceVectorEntry>&
DistanceVectorTable::getEntries() const {

    return entries;
}

void DistanceVectorTable::clear() {
    entries.clear();
}

void DistanceVectorTable::display() const {

    std::cout
        << "\n===== DISTANCE VECTOR TABLE =====\n";

    if(owner)
        std::cout
            << "Router: "
            << owner->getName()
            << '\n';

    if(entries.empty()) {
        std::cout
            << "Table is empty.\n";
        return;
    }

    std::cout
        << "Destination\tNext Hop\tCost\n";

    for(const auto& item:entries) {

        const auto& entry=item.second;

        std::cout
            << entry.destination->getName()
            << '\t';

        if(entry.nextHop)
            std::cout
                << entry.nextHop->getName();
        else
            std::cout
                << "-";

        std::cout
            << '\t'
            << entry.cost
            << '\n';
    }
}