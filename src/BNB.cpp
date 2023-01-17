#include "BNB.h"

#include <utility>

// CONSTRUCTORS/DESTRUCTORS:
BNB::BNB() = default;
BNB::BNB(const Graph &graph) {
    this->graph = graph;
    this->ncities = graph.getNcities();
    this->bestRoute = new int[ncities];
}
BNB::~BNB() = default;

// GETTERS:
int *BNB::getBestRoute() const {
    return bestRoute;
}
void BNB::setBestRoute(int *route) {
    BNB::bestRoute = route;
}
const Graph &BNB::getGraph() const {
    return graph;
}

// METHODS:
bool BNB::isRouteBetter(int *route) {
    int routeCost = this->graph.getTravelCost(route);
    return routeCost < this->bestRouteCost;
}

// ADV. METHODS:
std::vector<std::vector<std::vector<int>>> BNB::getFirstPaths(int npes) const {
    // If there are more nodes than processes, we need to give each process more than one path:
    if (npes < int(getGraph().getNodes().size())) {
        int pathsPerProcess = int((getGraph().getNodes().size()) / npes) + 1;
        std::vector<std::vector<std::vector<int>>> paths(npes, std::vector<std::vector<int>>(pathsPerProcess, std::vector<int>(1, 0)));
        for (int i = 0; i < getGraph().getNodes().size(); i++) {
            paths[i % npes][i / npes][0] = i;
        }
        // Remove the empty paths:
        for (auto & path : paths) {
            if (path[path.size()-1][0] == 0) {
                path.pop_back();
            }
        }
        return paths;
    }
    // However, if there are less nodes than processes, we need go deeper and give each process more than one path:
    else {
        int pathsAmount = (int) getGraph().getDistances().size();
        int depth = 1;
        while (pathsAmount < npes) {
            pathsAmount *= pathsAmount;
            depth+=1;
        }
        std::vector<std::vector<int>> paths = std::vector<std::vector<int>>(pathsAmount, std::vector<int>(depth, 0));
        for (int i = 0; i < getGraph().getDistances().size(); i++) {
            for (int j = 0; j < getGraph().getDistances().size(); j++) {
                paths[i*getGraph().getDistances().size()+j][0] = i;
                paths[i*getGraph().getDistances().size()+j][1] = j;
            }
        }
        std::vector<std::vector<std::vector<int>>> pathsPerProcess = std::vector<std::vector<std::vector<int>>>(npes, std::vector<std::vector<int>>((pathsAmount/npes)+1, std::vector<int>(depth, 0)));
        // Split paths into pathsPerProcess, so that each process has approximately the same amount of paths to search.
        for (int i = 0; i < paths.size(); i++) {
            pathsPerProcess[i%npes][i/npes] = paths[i];
        }
        // Remove the empty paths from the end of each process' pathsPerProcess.
        for (auto & pathsPerProces : pathsPerProcess) {
            if (pathsPerProces[pathsPerProces.size()-1][0] == 0) {pathsPerProces.pop_back();}
        }
        return pathsPerProcess;
    }
}

int*** BNB::getFirstPaths(int npes) const {

}

void BNB::search(std::vector<int> path) {
    if(path.empty()) {
        for (int node : getGraph().getNodes()) {
            std::vector<int> newPath = path;
            newPath.push_back(node);
            this->search(newPath);
        }
    }
    else if(path.size() == ncities) {
        if (this->isRouteBetter(path)) {
            this->bestRoute = path;
        }
    }
    else {
        if (this->isRouteBetter(path)) {
            for (int node : getGraph().getNodes()) {
                if (find(path.begin(), path.end(), node) == path.end()) {
                    std::vector<int> newPath = path;
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
    std::cout << "Best route: ";
    for (int i = 0; i < ncities; i++) {
        std::cout << bestRoute[i] << " ";
    }
    std::cout << std::endl;
}
void BNB::bestCostToString() {
    std::cout << "Best cost: " << this->graph.getTravelCost(this->bestRoute) << std::endl;
}
