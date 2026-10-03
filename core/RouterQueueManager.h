#ifndef ROUTERQUEUEMANAGER_H
#define ROUTERQUEUEMANAGER_H
#include "NetworkQueue.h"
#include <map>
#include <string>
class RouterQueueManager{
private:
    std::map<std::string,NetworkQueue> queues;
    size_t defaultCapacity;
public:
    RouterQueueManager(size_t defaultCapacity=10);
    void createQueue(const std::string& interfaceName,size_t capacity=0);
    bool hasQueue(const std::string& interfaceName) const;
    NetworkQueue& getQueue(const std::string& interfaceName);
    void removeQueue(const std::string& interfaceName);
    void clear();
    void display() const;
};
#endif