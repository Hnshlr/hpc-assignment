#include "Graph.h"

// CONSTRUCTORS/DESTRUCTORS:
Graph::Graph() {}
Graph::Graph(const std::string &distFilename) {
    // Open the distFile:
    std::ifstream distFile(distFilename);
    if (!distFile.is_open()) {
        std::cout << "Could not open distFile: " << distFilename << ".\n";
        exit(1);
    }

    // Read the distFile line by line:
    std::vector<std::vector<std::string>> elements;
    std::string line;
    while (getline(distFile, line)) {
        // Split the line by spaces:
        std::vector<std::string> lineElements;
        std::istringstream iss(line);
        for (std::string s; iss >> s;) {
            lineElements.push_back(s);
        }
        // Add the line to the elements:
        elements.push_back(lineElements);
    }

    // Create the distances vector:
    this->distances = std::vector<std::vector<int>>(elements.size(), std::vector<int>(elements.size(), 0));
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

    // Create the nodes vector:
    this->nodes = std::vector<int>(this->distances.size(), 0);
    for (int i = 0; i < this->nodes.size(); i++) {this->nodes[i] = i;}
}
Graph::~Graph() = default;

// GETTERS:
const std::vector<int> &Graph::getNodes() const {
    return nodes;
}
const std::vector<std::vector<int>> &Graph::getDistances() const {
    return distances;
}

// ADV. GETTERS:
int Graph::getDistance(int i, int j) {
    return this->distances[i][j];
}
int Graph::getTravelCost(std::vector<int> route) {
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
            std::cout << getDistance(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

