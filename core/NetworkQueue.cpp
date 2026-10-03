#include "NetworkQueue.h"
#include <iostream>
NetworkQueue::NetworkQueue(size_t capacity):capacity(capacity),totalEnqueued(0),totalDequeued(0),totalDropped(0),maximumSize(0),totalQueueDelay(0),totalSize(0){}
bool NetworkQueue::enqueue(const QueuedPacket& packet){
    if(full()){++totalDropped;return false;}
    packets.push(packet);
    ++totalEnqueued;
    if(packets.size()>maximumSize)maximumSize=packets.size();
    totalSize+=packet.size;
    return true;
}
bool NetworkQueue::dequeue(QueuedPacket& packet){
    if(empty())return false;
    packet=packets.front();
    packets.pop();
    ++totalDequeued;
    totalQueueDelay+=packet.serviceTime-packet.arrivalTime;
    totalSize-=packet.size;
    return true;
}
bool NetworkQueue::empty() const{return packets.empty();}
bool NetworkQueue::full() const{return packets.size()>=capacity;}
size_t NetworkQueue::size() const{return packets.size();}
size_t NetworkQueue::getCapacity() const{return capacity;}
size_t NetworkQueue::getMaximumSize() const{return maximumSize;}
int NetworkQueue::getTotalEnqueued() const{return totalEnqueued;}
int NetworkQueue::getTotalDequeued() const{return totalDequeued;}
int NetworkQueue::getTotalDropped() const{return totalDropped;}
double NetworkQueue::getTotalQueueDelay() const{return totalQueueDelay;}
double NetworkQueue::getAverageQueueDelay() const{return totalDequeued==0?0:totalQueueDelay/totalDequeued;}
double NetworkQueue::getUtilization() const{return capacity==0?0:(static_cast<double>(packets.size())/capacity)*100.0;}
void NetworkQueue::clear(){while(!packets.empty())packets.pop();totalEnqueued=0;totalDequeued=0;totalDropped=0;maximumSize=0;totalQueueDelay=0;totalSize=0;}
void NetworkQueue::display() const{
    std::cout<<"\n===== NETWORK QUEUE =====\n";
    std::cout<<"Capacity          : "<<capacity<<'\n';
    std::cout<<"Current Size      : "<<packets.size()<<'\n';
    std::cout<<"Maximum Size      : "<<maximumSize<<'\n';
    std::cout<<"Total Enqueued    : "<<totalEnqueued<<'\n';
    std::cout<<"Total Dequeued    : "<<totalDequeued<<'\n';
    std::cout<<"Total Dropped     : "<<totalDropped<<'\n';
    std::cout<<"Average Delay     : "<<getAverageQueueDelay()<<" ms\n";
    std::cout<<"Utilization       : "<<getUtilization()<<"%\n";
}