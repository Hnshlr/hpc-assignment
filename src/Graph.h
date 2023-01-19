#ifndef HPC_ASSIGNMENT_GRAPH_H
#define HPC_ASSIGNMENT_GRAPH_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define xncities 19     // Fixed number of cities (For compilation purposes).

class Graph {
private:
    // ATTRIBUTES:
    int *nodes = new int[xncities];
    int **distances = new int*[xncities];
    int ncities = xncities;

public:
    // CONSTRUCTORS/DESTRUCTORS:
    Graph();
    explicit Graph(const std::string &distFilename);
    virtual ~Graph();

    // GETTERS:
    [[nodiscard]] int getNcities() const;
    [[nodiscard]] int *getNodes() const;
    [[nodiscard]] int **getDistances() const;

    // ADV. GETTERS:
    int getDistance(int i, int j);
    int getTravelCost(const int *route);

    // METHODS:
    // ...

    // OTHERS:
    void toString();
};


#endif //HPC_ASSIGNMENT_GRAPH_H
