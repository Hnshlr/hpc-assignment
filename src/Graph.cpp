#include "Graph.h"

// CONSTRUCTORS/DESTRUCTORS:
Graph::Graph() {}
Graph::Graph(const string &distFilename) {
    // Open the distFile:
    ifstream distFile(distFilename);
    if (!distFile.is_open()) {
        cout << "Could not open distFile: " << distFilename << ".\n";
        exit(1);
    }

    // Read the distFile line by line:
    vector<vector<string>> elements;
    string line;
    while (getline(distFile, line)) {
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
                int element = stoi(elements[i][j]);
                this->distances[i][j] = element;
                this->distances[j][i] = element;
            }
        }
    }

    // Close the distFile:
    distFile.close();
}
Graph::~Graph() = default;

// GETTERS:
const vector<vector<int>> &Graph::getDistances() const {
    return distances;
}

// ADV. GETTERS:
int Graph::getDistance(int i, int j) {
    return this->distances[i][j];
}
int Graph::getTravelCost(vector<int> route) {
    int cost = 0;
    for (int i = 0; i < route.size()-1; i++) {
        cost += this->getDistance(route[i], route[i+1]);
    }
    return cost;
}

// METHODS:
int Graph::getAllPossibleRoutesAmount() {
    int n = this->distances.size();
    int amount = 1;
    for (int i = n; i > 1; i--) {
        amount *= i;
    }
    return amount;
}

// OTHERS:
void Graph::toString() {
    for (int i = 0; i < this->distances.size(); i++) {
        for (int j = 0; j < this->distances[i].size(); j++) {
            cout << getDistance(i, j) << " ";
        }
        cout << endl;
    }
}

