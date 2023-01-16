#ifndef HPC_ASSIGNMENT_BNB_H
#define HPC_ASSIGNMENT_BNB_H

#include "Graph.h"
#include <vector>


class BNB {
private:
    // ATTRIBUTES:
    Graph graph;
    vector<int> nodes;

    // ADV. ATTRIBUTES:
    vector<int> bestRoute;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    BNB();
    explicit BNB(const Graph &graph);
    virtual ~BNB();

    // GETTERS:
    [[nodiscard]] const Graph &getGraph() const;
    [[nodiscard]] const vector<int> &getNodes() const;
    [[nodiscard]] const vector<int> &getBestRoute() const;

    // METHODS:
    static vector<int> findShortestRoute();
    bool isRouteBetter(vector<int> route);
    void search(vector<int> path);

    // OTHERS:
    void bestRouteToString();
    void bestCostToString();
};


#endif //HPC_ASSIGNMENT_BNB_H
