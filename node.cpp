#include "ibex.h"
#include "node.h"

using namespace ibex;
using namespace std;

Node::Node(): _value(IntervalVector::empty(2))
{

}

Node::Node(IntervalVector value): _value(value)
{

}

vector<Node> Node::getNeighbor() const{
    return _neighbor;
}

IntervalVector Node::getValue() const{
    return _value;
}

void Node::addNeighbor(IntervalVector newNeighbor){

    _neighbor.push_back(newNeighbor);
}

void Node::setValue(IntervalVector newValue){
    _value = newValue;
}