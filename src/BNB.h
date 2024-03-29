#ifndef HPC_ASSIGNMENT_BNB_H
#define HPC_ASSIGNMENT_BNB_H

#include "Graph.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <mpi.h>
#include <string>

class BNB {
private:
    // ATTRIBUTES:
    Graph graph;
    int ncities = xncities;

    // ADV. ATTRIBUTES:
    int *bestRoute{};
    int bestRouteCost = INT32_MAX;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    BNB();
    explicit BNB(const Graph &graph);
    virtual ~BNB();

    // GETTERS & SETTERS:
    [[nodiscard]] int getNcities() const;
    [[nodiscard]] const Graph &getGraph() const;
    [[nodiscard]] int *getBestRoute() const;
    [[nodiscard]] int getBestRouteCost() const;
    void setBestRoute(int *bestRoute);
    void setBestRouteCost(int bestRouteCost);

    // METHODS:
    // ...

    // ADV. METHODS:
    [[nodiscard]] std::vector<std::vector<std::vector<int>>> getFirstPaths(int npes, int startingNode) const;
    [[nodiscard]] std::vector<std::vector<std::vector<int>>> getFirstPathsV2(int npes, int startingNode) const;
    void setBestRouteUsingUniformCostSearch(int startingNode);
    void search(int *path, int pathSize, int cost, int *visited);
    std::tuple<double, double> searchMPI(int *path, int pathSize, int cost, int *visited, int myrank, int npes, double start);


    // OTHERS:
    [[nodiscard]] std::string bestRouteToString() const;
};


#endif //HPC_ASSIGNMENT_BNB_H
