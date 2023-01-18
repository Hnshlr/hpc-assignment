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
    if (npes < ncities) {
        int pathsPerProcess = int((ncities - 1) / npes);
        std::vector<std::vector<std::vector<int>>> paths(npes, std::vector<std::vector<int>>(pathsPerProcess, std::vector<int>(1, 0)));
        for (int i = 0; i < ncities; i++) {
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
        int pathsAmount = ncities;
        int depth = 1;
        while (pathsAmount < npes) {
            pathsAmount *= pathsAmount;
            depth+=1;
        }
        std::vector<std::vector<int>> paths = std::vector<std::vector<int>>(pathsAmount, std::vector<int>(depth, 0));
        for (int i = 0; i < ncities; i++) {
            for (int j = 0; j < ncities; j++) {
                paths[i * ncities + j][0] = i;
                paths[i * ncities + j][1] = j;
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
void BNB::search(int *path, int pathSize, int cost, int *visited) {
    // If all cities have been visited, check if the path is better than the best path found so far.
    if (pathSize == ncities) {
        if (cost < bestRouteCost) {
            bestRouteCost = cost;
            for (int i = 0; i < ncities; i++) {
                bestRoute[i] = path[i];
            }
        }
        return;
    }
    if (cost > bestRouteCost) {
        return;
    }
    // If the path size is 0, it means no city was selected to start with. Therefore test all cases for the first node:
    if (pathSize == 0) {
        for (int i = 0; i < ncities; i++) {
            visited[i] = 1;
            path[pathSize] = i;
            search(path, pathSize + 1, cost, visited);
            visited[i] = 0;
        }
    }
    // If the path is uncomplete, fill out the path with unvisited cities:
    else {
        for (int i = 0; i < ncities; i++) {
            if (visited[i] == 0) {
                visited[i] = 1;
                path[pathSize] = i;
                search(path, pathSize + 1, cost + graph.getDistance(path[pathSize - 1], i), visited);
                visited[i] = 0;
            }
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
void BNB::bestCostToString() const {
    std::cout << "Best cost: " << bestRouteCost << std::endl;
}
