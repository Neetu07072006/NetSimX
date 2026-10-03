#ifndef NETWORKQUEUE_H
#define NETWORKQUEUE_H
#include <queue>
#include <string>
struct QueuedPacket{
    int packetId;
    int size;
    std::string protocol;
    double arrivalTime;
    double serviceTime;
};
class NetworkQueue{
private:
    std::queue<QueuedPacket> packets;
    size_t capacity;
    int totalEnqueued;
    int totalDequeued;
    int totalDropped;
    size_t maximumSize;
    double totalQueueDelay;
    double totalSize;
public:
    NetworkQueue(size_t capacity=10);
    bool enqueue(const QueuedPacket& packet);
    bool dequeue(QueuedPacket& packet);
    bool empty() const;
    bool full() const;
    size_t size() const;
    size_t getCapacity() const;
    size_t getMaximumSize() const;
    int getTotalEnqueued() const;
    int getTotalDequeued() const;
    int getTotalDropped() const;
    double getTotalQueueDelay() const;
    double getAverageQueueDelay() const;
    double getUtilization() const;
    void clear();
    void display() const;
};
#endif