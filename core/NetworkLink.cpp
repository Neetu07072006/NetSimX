#include "NetworkLink.h"
#include "NetworkNode.h"
#include <iostream>
NetworkLink::NetworkLink(NetworkNode* nodeA,NetworkNode* nodeB,double bandwidth,double latency,double packetLoss):nodeA(nodeA),nodeB(nodeB),bandwidth(bandwidth),latency(latency),packetLoss(packetLoss),active(true){}
NetworkNode* NetworkLink::getNodeA() const{return nodeA;}
NetworkNode* NetworkLink::getNodeB() const{return nodeB;}
double NetworkLink::getBandwidth() const{return bandwidth;}
double NetworkLink::getLatency() const{return latency;}
double NetworkLink::getPacketLoss() const{return packetLoss;}
bool NetworkLink::isActive() const{return active;}
void NetworkLink::setBandwidth(double bandwidth){this->bandwidth=bandwidth;}
void NetworkLink::setLatency(double latency){this->latency=latency;}
void NetworkLink::setPacketLoss(double packetLoss){this->packetLoss=packetLoss;}
void NetworkLink::setActive(bool active){this->active=active;}
void NetworkLink::fail(){active=false;}
void NetworkLink::recover(){active=true;}
void NetworkLink::display() const{std::cout<<nodeA->getName()<<" <-> "<<nodeB->getName()<<" | Bandwidth: "<<bandwidth<<" Mbps | Latency: "<<latency<<" ms | Loss: "<<packetLoss<<"% | Status: "<<(active?"ACTIVE":"FAILED")<<'\n';}