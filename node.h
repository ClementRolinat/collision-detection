#include "ibex.h"

class Node
{
private:
    ibex::IntervalVector _value;
    std::vector<Node*> _neighbor;
    Node* _pred;
    double _dist;

public:
    Node();
    Node(ibex::IntervalVector value);
    std::vector<Node*> getNeighbor() const;
    ibex::IntervalVector getValue() const;
    void addNeighbor(Node* newNeighbor);
    void setValue(ibex::IntervalVector newValue);
    void setDist(double newDist);
    void setPred(Node* newPred);
    double getDist() const;
    Node* getPred() const;
};