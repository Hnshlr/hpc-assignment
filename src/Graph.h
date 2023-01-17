#ifndef HPC_ASSIGNMENT_GRAPH_H
#define HPC_ASSIGNMENT_GRAPH_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Graph {
private:
    // ATTRIBUTES:
    int *nodes = new int[ncities];
    int **distances = new int*[ncities];
    int ncities{};

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
