#include <iostream>
#include <mpi.h>
#include <random>

#include "src/Graph.h"
#include "src/BNB.h"

int main(int argc, char *argv[]) {

    // INITIALIZE THE MPI ENVIRONMENT:
    int npes, myrank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0) printf("Initialized the MPI environment with %d process%s.\n", npes, npes > 1 ? "es" : "");

    // TIMER START:
    double start = MPI_Wtime();
    std::tuple<double, double> commTimes = {0, 0};

    // SETTINGS:
    std::string distFilename = argv[1];

    // GRAPH AND BRANCH-AND-BOUND OBJECTS INITIALIZATION:
    Graph graph = *new Graph(distFilename);
    BNB bnb = *new BNB(graph);
    int ncities = bnb.getNcities();

    // PREFERENCES:
    int path[ncities];                  // Path to be computed
    int cost = 0;                       // Cost of the path
    int visited[xncities] = {0};        // Visited nodes
    if (myrank == 0) {                  // Root process initializes the path, by selecting a random node to start with
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, ncities - 1);
        path[0] = dis(gen);
        if (argv[2] != nullptr) {       // If a starting node is specified, select a random node instead
            path[0] = std::stoi(argv[2]);
        }
        visited[path[0]] = 1;           // And mark it as visited
        printf("PROCESS %d: Selected starting node: %d\n", myrank, path[0]);
    }

    // BROADCAST THE STARTING NODE:
    MPI_Bcast(path, ncities, MPI_INT, 0, MPI_COMM_WORLD);


    // FIND EACH PROCESS' STARTING PATHS, DEPENDING ON THE AMOUNT OF PROCESSES AND THE RANK OF THE CURRENT PROCESS:
    std::vector<std::vector<std::vector<int>>> pathsVectors = bnb.getFirstPathsV2(npes, path[0]);
    int totalAmountOfPaths = 0;     // Total amount of paths to be computed
    for (const auto & pathsVector : pathsVectors) totalAmountOfPaths += (int) pathsVector.size();
    int amountOfPaths = (int) pathsVectors[myrank].size();    // Amount of paths to be computed by the current process
    int amountOfNodesPerPath = (int) pathsVectors[myrank][0].size();    // Amount of nodes per path
    int **paths = new int *[amountOfPaths];                 // Pointer to the paths to be computed by the current process
    for (int i = 0; i < amountOfPaths; i++) {               // Allocate memory for the paths
        paths[i] = new int[amountOfNodesPerPath];           // Allocate memory for the nodes of each path
        for (int j = 0; j < amountOfNodesPerPath; j++) {    // Fill the paths with the data from the pathsVectors
            paths[i][j] = pathsVectors[myrank][i][j];       // Fill the paths with the data from the pathsVectors
        }
    }
    bnb.setBestRouteUsingUniformCostSearch(path[0]);    // Experimental: set the initial best route using Uniform Cost Search

    // MAIN JOB: COMPUTE THE SEARCH ON EACH PROCESS:
    for (int i = 0; i < amountOfPaths; i++) {   // For each path to be computed by the current process
        cost = 0;                                           // Reset the cost
        for (int j = 0; j < amountOfNodesPerPath; j++) {    // For each node of the current path
            visited[paths[i][j]] = 1;                       // Mark the node as visited
            path[j] = paths[i][j];                          // Update the path
            if (j > 0) {                                    // If the node is not the first one
                cost += graph.getDistance(path[j - 1], path[j]);        // Compute the cost of the already begun path
            }
        }
        // TODO: WORK/OPTIMISATION IN PROGRESS:
        // If the amount of paths to be computed is divisible by the amount of processes, or if it's not, but the current
        // path is not the last one to be computed, then share the results with the other processes:
        if ((totalAmountOfPaths%npes) == 0 && i<(totalAmountOfPaths/npes)-1
        || (totalAmountOfPaths%npes) != 0 && i<(totalAmountOfPaths/npes)) {
            // Compute the search and share results with the other processes after each path is fully computed:
            std::tuple<double, double> temp = bnb.searchMPI(path, amountOfNodesPerPath, cost, visited, myrank, npes, start);
            // Update the communication and idling times:
            commTimes = {std::get<0>(commTimes) + std::get<0>(temp), std::get<1>(commTimes) + std::get<1>(temp)};
        }
        // However, if the current path is the last one to be computed, then don't share results with the other processes:
        else {
            // Compute the search, but since it's the last path to be computed, don't share results with the other processes:
            bnb.search(path, amountOfNodesPerPath, cost, visited);
        }
        // TODO: END OF WORK IN PROGRESS.
        // Reset the visited nodes for the next path:
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            visited[paths[i][j]] = 0;
        }
    }
    // Measure the time spent in the last communication:
    double in, lastIn, out;
    // Now that every process is out of the loop, the final sharing of results can occur:
    int bestCostAndRank[2] = {bnb.getBestRouteCost(), myrank};
    in = MPI_Wtime() - start;
    // Gather one last time the best cost, and the rank of the process that found it:
    MPI_Allreduce(MPI_IN_PLACE, bestCostAndRank, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
    // Broadcast the best route to all processes, using the rank of the process that found it:
    MPI_Bcast(bnb.getBestRoute(), ncities, MPI_INT, bestCostAndRank[1], MPI_COMM_WORLD);
    out = MPI_Wtime() - start;
    bnb.setBestRouteCost(bestCostAndRank[0]);
    // Gather the first and last communication times, to calculate the idle time:
    MPI_Allreduce(MPI_IN_PLACE, &in, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&in, &lastIn, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE, &out, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    commTimes = {std::get<0>(commTimes) + out - in, std::get<1>(commTimes) + out - lastIn};
    double idleTime = std::get<0>(commTimes);
    double commTime = std::get<1>(commTimes);


    // MAKE THE PROCESS 0 ANNOUNCE THE BEST COST AND ROUTE:
    if (myrank == 0) {
        printf("PROCESS %d: The best route cost is: %d.\n", myrank, bnb.getBestRouteCost());
        printf("PROCESS %d: The best route is: %s\n", myrank, bnb.bestRouteToString().c_str());
        printf("Total computation took: %f seconds.\n", ((double) ((MPI_Wtime() - start) * 10000) / 10000.0));
        printf("Total idling time: %f seconds.\n", ((double) ((idleTime) * 10000) / 10000.0));
        printf("Total communication time: %f seconds.\n", ((double) ((commTime) * 10000) / 10000.0));
    }

    // FINALIZE THE MPI ENVIRONMENT:
    MPI_Finalize();

}
