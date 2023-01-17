#ifndef HPC_ASSIGNMENT_BNB_H
#define HPC_ASSIGNMENT_BNB_H

#include "Graph.h"

#include <vector>
#include <algorithm>
#include <mpi.h>

class BNB {
private:
    // ATTRIBUTES:
    Graph graph;
    int ncities{};

    // ADV. ATTRIBUTES:
    std::vector<int> bestRoute;
    int bestRouteCost = INT_MAX;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    BNB();
    explicit BNB(const Graph &graph);
    virtual ~BNB();

    // GETTERS & SETTERS:
    [[nodiscard]] const Graph &getGraph() const;
    [[nodiscard]] const std::vector<int> &getBestRoute() const;
    void setBestRoute(const std::vector<int> &route);

    // METHODS:
    bool isRouteBetter(std::vector<int> route);

    // ADV. METHODS:
    [[nodiscard]] std::vector<std::vector<std::vector<int>>> getFirstPaths(int npes) const;
    void search(std::vector<int> path);


    // OTHERS:
    void bestRouteToString();
    void bestCostToString();
};


#endif //HPC_ASSIGNMENT_BNB_H
