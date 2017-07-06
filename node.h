#include "ibex.h"

class Node
{
private:
    ibex::IntervalVector _value;
    std::vector<Node> _neighbor;

    Node();
    Node(ibex::IntervalVector value);
    std::vector<Node> getNeighbor() const;
    ibex::IntervalVector getValue() const;
    void addNeighbor(ibex::IntervalVector newNeighbor);
    void setValue(ibex::IntervalVector newValue);
};