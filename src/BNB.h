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
    int *bestRoute{};
    int bestRouteCost = INT_MAX;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    BNB();
    explicit BNB(const Graph &graph);
    virtual ~BNB();

    // GETTERS & SETTERS:
    [[nodiscard]] const Graph &getGraph() const;
    [[nodiscard]] int *getBestRoute() const;
    void setBestRoute(int *bestRoute);

    // METHODS:
    bool isRouteBetter(int *route);

    // ADV. METHODS:
    [[nodiscard]] std::vector<std::vector<std::vector<int>>> getFirstPaths(int npes) const;
    void search(int *path, int pathSize, int cost, int *visited);

    // OTHERS:
    void bestRouteToString();
    void bestCostToString() const;
};


#endif //HPC_ASSIGNMENT_BNB_H
