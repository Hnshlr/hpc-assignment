#ifndef HPC_ASSIGNMENT_MAP_H
#define HPC_ASSIGNMENT_MAP_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Map {
private:
    // ATTRIBUTES:
    vector<vector<int>> distances;
    int possibleRoutesAmount;
public:
    // CONSTRUCTORS/DESTRUCTORS:
    explicit Map(const string &filename);
    virtual ~Map();

    // GETTERS
    int getPossibleRoutesAmount() const;

    // ADV. GETTERS:
    int getDistance(int i, int j);
    int getRouteCost(vector<int> route);

    // OTHERS:
    void toString();
};


#endif //HPC_ASSIGNMENT_MAP_H
