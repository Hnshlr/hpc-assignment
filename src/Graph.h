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
    std::vector<std::vector<int>> distances;
    std::vector<int> nodes;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    Graph();
    explicit Graph(const std::string &distFilename);
    virtual ~Graph();

    // GETTERS:
    [[nodiscard]] const std::vector<int> &getNodes() const;
    [[nodiscard]] const std::vector<std::vector<int>> &getDistances() const;

    // ADV. GETTERS:
    int getDistance(int i, int j);
    int getTravelCost(std::vector<int> route);

    // METHODS:
    int getAllPossibleRoutesAmount();

    // OTHERS:
    void toString();
};


#endif //HPC_ASSIGNMENT_GRAPH_H
