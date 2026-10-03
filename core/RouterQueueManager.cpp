#include "RouterQueueManager.h"
#include <iostream>
RouterQueueManager::RouterQueueManager(size_t defaultCapacity):defaultCapacity(defaultCapacity){}
void RouterQueueManager::createQueue(const std::string& interfaceName,size_t capacity){
    if(capacity==0)capacity=defaultCapacity;
    if(!hasQueue(interfaceName))queues.emplace(interfaceName,NetworkQueue(capacity));
}
bool RouterQueueManager::hasQueue(const std::string& interfaceName) const{return queues.find(interfaceName)!=queues.end();}
NetworkQueue& RouterQueueManager::getQueue(const std::string& interfaceName){
    if(!hasQueue(interfaceName))createQueue(interfaceName);
    return queues.at(interfaceName);
}
void RouterQueueManager::removeQueue(const std::string& interfaceName){queues.erase(interfaceName);}
void RouterQueueManager::clear(){queues.clear();}
void RouterQueueManager::display() const{
    std::cout<<"\n===== ROUTER QUEUES =====\n";
    if(queues.empty()){std::cout<<"No queues configured.\n";return;}
    for(const auto& entry:queues){
        std::cout<<"\nInterface: "<<entry.first<<'\n';
        entry.second.display();
    }
}