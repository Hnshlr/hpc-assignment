#include "Map.h"

Map::Map(const string &filename) {
    // Open the file:
    ifstream file(filename);
    // Check if the file is open:
    if (!file.is_open()) {
        cout << "Could not open file: " << filename << ".\n";
        cout << ::strerror(errno) << endl;
        exit(1);
    }
    // Read the file line by line:
    vector<vector<string>> elements;
    string line;
    while (getline(file, line)) {
        // Split the line by spaces:
        vector<string> lineElements;
        istringstream iss(line);
        for (string s; iss >> s;) {
            lineElements.push_back(s);
        }
        // Add the line to the elements:
        elements.push_back(lineElements);
    }

    // Create the distances vector:
    this->distances = vector<vector<int>>(elements.size(), vector<int>(elements.size(), 0));
    for (int i = 1; i < elements.size(); i++) {
        for (int j = 0; j < elements[i].size(); j++) {
            if (i==j) {
                this->distances[i][j] = 0;
            }
            else {
                // Convert each element of elements to an int:
                int element = stoi(elements[i][j]);
                this->distances[i][j] = element;
                this->distances[j][i] = element;
            }
        }
    }

    // Deduce the amount of possible routes using the factorial function:
    int n = this->distances.size();
    for (int i = 1; i < n; i++) {
        this->possibleRoutesAmount *= i;
    }
}
Map::~Map() = default;


int Map::getDistance(int i, int j) {
    return this->distances[i][j];
}

int Map::getRouteCost(vector<int> route) {
    int cost = 0;
    for (int i = 0; i < route.size()-1; i++) {
        cost += this->getDistance(route[i], route[i+1]);
    }
    return cost;
}

void Map::toString() {
    for (int i = 0; i < this->distances.size(); i++) {
        for (int j = 0; j < this->distances[i].size(); j++) {
            cout << getDistance(i, j) << " ";
        }
        cout << endl;
    }
}

int Map::getPossibleRoutesAmount() const {
    return possibleRoutesAmount;
}
