#include "BNB.h"

// CONSTRUCTORS/DESTRUCTORS:
BNB::BNB() {}
BNB::BNB(const Graph &graph) {
    this->graph = graph;
    this->nodes = vector<int>(graph.getDistances().size(), 0);
    for (int i = 0; i < this->nodes.size(); i++) {this->nodes[i] = i;}
    this->bestRoute = vector<int>(this->nodes);
}
BNB::~BNB() {}

// GETTERS:
const Graph &BNB::getGraph() const {
    return graph;
}
const vector<int> &BNB::getNodes() const {
    return nodes;
}
const vector<int> &BNB::getBestRoute() const {
    return bestRoute;
}

// METHODS:
vector<int> BNB::findShortestRoute() {
    return {};
}
bool BNB::isRouteBetter(vector<int> route) {
    return this->graph.getTravelCost(route) < this->graph.getTravelCost(this->bestRoute);
}


void BNB::search(vector<int> path) {
    if(path.empty()) {
        for (int node : this->nodes) {
            vector<int> newPath = path;
            newPath.push_back(node);
            this->search(newPath);
        }
    }
    else if(path.size() == this->graph.getDistances().size()) {
        if (this->isRouteBetter(path)) {
            this->bestRoute = path;
        }
    }
    else {
        if (this->isRouteBetter(path)) {
            for (int node : this->nodes) {
                if (find(path.begin(), path.end(), node) == path.end()) {
                    vector<int> newPath = path;
                    newPath.push_back(node);
                    this->search(newPath);
                }
            }
        }
        else {
            return;
        }
    }
}

// OTHERS:
void BNB::bestRouteToString() {
    cout << "Best route: ";
    for (int i : this->bestRoute) {
        cout << i << " ";
    }
    cout << endl;
}
void BNB::bestCostToString() {
    cout << "Best cost: " << this->graph.getTravelCost(this->bestRoute) << endl;
}

