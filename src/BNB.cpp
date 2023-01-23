#include "BNB.h"

// CONSTRUCTORS/DESTRUCTORS:
BNB::BNB() = default;
BNB::BNB(const Graph &graph) {
    this->graph = graph;
    this->ncities = graph.getNcities();
    this->bestRoute = new int[ncities];
}
BNB::~BNB() = default;

// GETTERS:
int BNB::getNcities() const {
    return ncities;
}
int *BNB::getBestRoute() const {
    return bestRoute;
}
int BNB::getBestRouteCost() const {
    return bestRouteCost;
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
std::vector<std::vector<std::vector<int>>> BNB::getFirstPaths(int npes, int startingNode) const {
    if(npes < ncities) {
        // Generate all possible paths, considering the starting node:
        std::vector<std::vector<int>> paths;
        for(int i = 0; i < ncities; i++) {
            if(i != startingNode) {
                std::vector<int> path;
                path.push_back(startingNode);
                path.push_back(i);
                paths.push_back(path);
            }
        }
        // Distribute paths between processes:
        int pathsPerProcessCount = (int) (paths.size() / npes) + 1;
        std::vector<std::vector<std::vector<int>>> pathsPerProcess = std::vector<std::vector<std::vector<int>>>(npes, std::vector<std::vector<int>>());
        for(int i = 0; i < paths.size(); i++) {
            pathsPerProcess[i % npes].push_back(paths[i]);
        }
        return pathsPerProcess;
    }
    else {
        // Find the depth of the tree, so that there are more paths than processes (considering the starting node):
        int depth = 1;
        int pathsCount = 1;
        while(pathsCount < npes) {
            pathsCount *= ncities - depth;
            depth++;
        }
        std::vector<std::vector<int>> paths = std::vector<std::vector<int>>(pathsCount, std::vector<int>(depth));
        // Find all combinations of depth-1 nodes, considering the starting node as the first node, and without repetitions:
        std::vector<int> nodesToAssign;
        for(int i = 0; i < ncities; i++) {
            if(i != startingNode) {
                nodesToAssign.push_back(i);
            }
        }
        int p = depth - 1;
        int n = (int) nodesToAssign.size();
        // Find all (n!)/(n-p)! permutations of p nodes:
        std::vector<std::vector<int>> permutations = std::vector<std::vector<int>>();
        std::vector<int> permutation = std::vector<int>(p);
        std::vector<bool> used = std::vector<bool>(n);
        std::function<void(int)> generatePermutations = [&](int index) {
            if(index == p) {
                permutations.push_back(permutation);
                return;
            }
            for(int i = 0; i < n; i++) {
                if(!used[i]) {
                    used[i] = true;
                    permutation[index] = nodesToAssign[i];
                    generatePermutations(index + 1);
                    used[i] = false;
                }
            }
        };
        generatePermutations(0);
        // Assign the permutations to the paths:
        for(int i = 0; i < pathsCount; i++) {
            paths[i][0] = startingNode;
            for(int j = 0; j < p; j++) {
                paths[i][j + 1] = permutations[i][j];
            }
        }
        // Remove permutations and permutation from memory:
        permutations.clear();
        permutation.clear();
        used.clear();
        // Distribute paths between processes:
        int pathsPerProcessCount = (int) (paths.size() / npes) + 1;
        std::vector<std::vector<std::vector<int>>> pathsPerProcess = std::vector<std::vector<std::vector<int>>>(npes, std::vector<std::vector<int>>());
        // Distribute it so that there's only a difference of 1 between the number of paths per process:
        for(int i = 0; i < pathsCount; i++) {
            pathsPerProcess[i % npes].push_back(paths[i]);
        }
        return pathsPerProcess;
    }
}
std::vector<std::vector<std::vector<int>>> BNB::getFirstPathsV2(int npes, int startingNode) const {
    // Find the depth of the tree, so that there are more paths than processes (considering the starting node):
    int depth = 1;
    int pathsCount = 1;
    while(pathsCount < npes || depth != 4) {
        pathsCount *= ncities - depth;
        depth++;
    }
    std::vector<std::vector<int>> paths = std::vector<std::vector<int>>(pathsCount, std::vector<int>(depth));
    // Find all combinations of depth-1 nodes, considering the starting node as the first node, and without repetitions:
    std::vector<int> nodesToAssign;
    for(int i = 0; i < ncities; i++) {
        if(i != startingNode) {
            nodesToAssign.push_back(i);
        }
    }
    int p = depth - 1;
    int n = (int) nodesToAssign.size();
    // Find all (n!)/(n-p)! permutations of p nodes:
    std::vector<std::vector<int>> permutations = std::vector<std::vector<int>>();
    std::vector<int> permutation = std::vector<int>(p);
    std::vector<bool> used = std::vector<bool>(n);
    std::function<void(int)> generatePermutations = [&](int index) {
        if(index == p) {
            permutations.push_back(permutation);
            return;
        }
        for(int i = 0; i < n; i++) {
            if(!used[i]) {
                used[i] = true;
                permutation[index] = nodesToAssign[i];
                generatePermutations(index + 1);
                used[i] = false;
            }
        }
    };
    generatePermutations(0);
    // Assign the permutations to the paths:
    for(int i = 0; i < pathsCount; i++) {
        paths[i][0] = startingNode;
        for(int j = 0; j < p; j++) {
            paths[i][j + 1] = permutations[i][j];
        }
    }
    // Remove permutations and permutation from memory:
    permutations.clear();
    permutation.clear();
    used.clear();
    // Distribute paths between processes:
    int pathsPerProcessCount = (int) (paths.size() / npes) + 1;
    std::vector<std::vector<std::vector<int>>> pathsPerProcess = std::vector<std::vector<std::vector<int>>>(npes, std::vector<std::vector<int>>());
    // Distribute it so that there's only a difference of 1 between the number of paths per process:
    for(int i = 0; i < pathsCount; i++) {
        pathsPerProcess[i % npes].push_back(paths[i]);
    }
    return pathsPerProcess;
}
void BNB::search(int *path, int pathSize, int cost, int *visited) {
    // Base case: If the path is empty (ie. no city was selected to start with), test all cases for the first node:
    if (pathSize == 0) {
        for (int i = 0; i < ncities; i++) {
            visited[i] = 1;
            path[pathSize] = i;
            if (cost + graph.getDistance(path[pathSize - 1], i) < bestRouteCost) {
                search(path, pathSize + 1, cost, visited);
            }
            visited[i] = 0;
        }
    }
    // If the path is complete, check if it's better than the best path found so far:
    else if (pathSize == ncities) {
        if (cost < bestRouteCost) {
            bestRouteCost = cost;
            for (int i = 0; i < ncities; i++) {
                bestRoute[i] = path[i];
            }
        }
    }
    // If the path is uncomplete, fill out the path with unvisited cities:
    else {
        for (int i = 0; i < ncities; i++) {
            if (visited[i] == 0) {
                visited[i] = 1;
                path[pathSize] = i;
                if (cost + graph.getDistance(path[pathSize - 1], i) < bestRouteCost) {
                    search(path, pathSize + 1, cost + graph.getDistance(path[pathSize - 1], i), visited);
                }
                visited[i] = 0;
            }
        }
    }
}
void BNB::searchMPI(int *path, int pathSize, int cost, int *visited, int myrank, int npes) {
    // TODO: WORK IN PROGRESS:
    search(path, pathSize, cost, visited);
    int bestCostAndRank[2] = {bestRouteCost, myrank};
    MPI_Allreduce(MPI_IN_PLACE, bestCostAndRank, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
    MPI_Bcast(bestRoute, ncities, MPI_INT, bestCostAndRank[1], MPI_COMM_WORLD);
    bestRouteCost = bestCostAndRank[0];
    // TODO: END OF WORK IN PROGRESS.
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
