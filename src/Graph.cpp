#include "Graph.h"

// CONSTRUCTORS/DESTRUCTORS:
Graph::Graph() = default;
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

    ncities = (int) elements.size();

    // Create the distances vector:
    this->distances = new int*[ncities];
    for (int i = 0; i < ncities; i++) {
        this->distances[i] = new int[ncities];
    }
    for (int i = 1; i < ncities; i++) {
        for (int j = 0; j < i; j++) {
            if (i==j) {
                this->distances[i][j] = 0;
            }
            else {
                this->distances[i][j] = std::stoi(elements[i][j]);
                this->distances[j][i] = std::stoi(elements[i][j]);
            }
        }
    }

    // Close the distFile:
    distFile.close();

    // Create the nodes vector:
    this->nodes = new int[ncities];
    for (int i = 0; i < ncities; i++) {
        this->nodes[i] = i;
    }
}
Graph::~Graph() = default;

// GETTERS:
int Graph::getNcities() const {
    return ncities;
}
int *Graph::getNodes() const {
    return nodes;
}
int **Graph::getDistances() const {
    return distances;
}

// ADV. GETTERS:
int Graph::getDistance(int i, int j) {
    return this->distances[i][j];
}
int Graph::getTravelCost(const int *route) {
    int cost = 0;
    for (int i = 0; i < ncities; i++) {
        cost+=this->distances[route[i]][route[i+1]];
    }
    return cost;
}

// METHODS:
// ...

// OTHERS:
void Graph::toString() {
    std::cout << "Graph:\n";
    std::cout << "ncities: " << ncities << "\n";
    std::cout << "nodes: ";
    for (int i = 0; i < ncities; i++) {
        std::cout << nodes[i] << " ";
    }
    std::cout << "\n";
    std::cout << "distances: \n";
    for (int i = 0; i < ncities; i++) {
        for (int j = 0; j < ncities; j++) {
            std::cout << distances[i][j] << " ";
        }
        std::cout << "\n";
    }
}

