#ifndef HPC_ASSIGNMENT_GRAPH_H
#define HPC_ASSIGNMENT_GRAPH_H


#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Graph {
private:
    // ATTRIBUTES:
    vector<vector<int>> distances;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    Graph();
    explicit Graph(const string &distFilename);
    virtual ~Graph();

    // GETTERS:
    [[nodiscard]] const vector<vector<int>> &getDistances() const;

    // ADV. GETTERS:
    int getDistance(int i, int j);
    int getTravelCost(vector<int> route);

    // METHODS:
    int getAllPossibleRoutesAmount();

    // OTHERS:
    void toString();
};


#endif //HPC_ASSIGNMENT_GRAPH_H
