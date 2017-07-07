#include "ibex.h"
#include "node.h"


using namespace ibex;
using namespace std;

Node::Node(): _value(IntervalVector::empty(2)), _dist(10000000), _pred(NULL)
{
    _neighbor.reserve(15);
}

Node::Node(IntervalVector value): _value(value), _dist(10000000), _pred(NULL)
{
    _neighbor.reserve(15);
}

vector<Node*> Node::getNeighbor() const{
    return _neighbor;
}

IntervalVector Node::getValue() const{
    return _value;
}

void Node::addNeighbor(Node* newNeighbor){
    _neighbor.push_back(newNeighbor);
}

void Node::setValue(IntervalVector newValue){
    _value = newValue;
}

void Node::setDist(double newDist){
    _dist = newDist;
}

void Node::setPred(Node* newPred){
    _pred = newPred;
}

double Node::getDist() const{
    return _dist;
}

Node* Node::getPred() const{
    return _pred;
}