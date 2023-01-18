#include <iostream>
#include <mpi.h>
#include <random>

#include "src/Graph.h"
#include "src/BNB.h"

int main(int argc, char *argv[]) {

    // INITIALIZE THE MPI ENVIRONMENT:
    MPI_Init(&argc, &argv);
    // AMOUNT OF PROCESSES, AND RANK OF THE CURRENT PROCESS:
    int npes, myrank;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // SETTINGS:
//    std::string distFilename = argv[1];
    std::string distFilename = "src/data/distances/dist17.txt";

    // MAIN:
    Graph graph = *new Graph(distFilename);
    BNB bnb = *new BNB(graph);

    // PREFERENCES:
    int path[xncities];              // Path to be computed
    int cost = 0;                   // Cost of the path
    int visited[xncities] = {0};     // Visited nodes
    if (myrank == 0) {              // Root process initializes the path, by selecting a random node to start with
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, xncities - 1);
        path[0] = dis(gen);
        visited[path[0]] = 1;
        printf("PROCESS %d: Selected starting node: %d\n", myrank, path[0]);
    }
    // BROADCAST THE STARTING NODE:
    MPI_Bcast(path, xncities, MPI_INT, 0, MPI_COMM_WORLD);

    // FIND EACH PROCESS' STARTING PATHS, DEPENDING ON THE AMOUNT OF PROCESSES AND THE RANK OF THE CURRENT PROCESS:
    std::vector<std::vector<std::vector<int>>> pathsVectors = bnb.getFirstPaths(npes, path[0]);
    int amountOfPaths = (int) pathsVectors[myrank].size();    // Amount of paths to be computed by the current process
    int amountOfNodesPerPath = (int) pathsVectors[myrank][0].size();    // Amount of nodes per path
    int **paths = new int *[amountOfPaths];               // Pointer to the paths to be computed by the current process
    for (int i = 0; i < amountOfPaths; i++) {       // Allocate memory for the paths
        paths[i] = new int[amountOfNodesPerPath];   // Allocate memory for the nodes of each path
        for (int j = 0; j < amountOfNodesPerPath; j++) {    // Fill the paths with the data from the pathsVectors
            paths[i][j] = pathsVectors[myrank][i][j];    // Fill the paths with the data from the pathsVectors
        }
    }
    printf("PROCESS %d: Was assigned %d paths to search from: [", myrank, amountOfPaths);
    for (int i = 0; i < amountOfPaths; i++) {
        printf("[");
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            printf("%d, ", paths[i][j]);
        }
        printf("], ");
    }
    printf("]\n");

    // WAIT FOR ALL PROCESSES TO BE READY:
    MPI_Barrier(MPI_COMM_WORLD);

    // TIMER START:
    double start = MPI_Wtime();

    // COMPUTE THE SEARCH ON EACH PROCESS:
    for (int i = 0; i < amountOfPaths; i++) {
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            visited[paths[i][j]] = 1;
            path[j] = paths[i][j];
        }
        bnb.search(path, amountOfNodesPerPath, cost, visited);
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            visited[paths[i][j]] = 0;
        }
    }
    printf("PROCESS %d: Finished searching. Here are my results:\n", myrank);
    bnb.bestRouteToString();
    bnb.bestCostToString();
    int bestRouteCost = bnb.getBestRouteCost();
    int bestRoute[xncities];
    for (int i = 0; i < xncities; i++) {
        bestRoute[i] = bnb.getBestRoute()[i];
    }
    // INDIVIDUAL TIMER END:
    printf("Computation took: %f seconds.\n", ((int) ((MPI_Wtime() - start) * 10000) / 10000.0));


    // WAIT FOR ALL PROCESSES TO BE READY:
    MPI_Barrier(MPI_COMM_WORLD);

    // MAKE ALL PROCESSES SEND THEIR BEST ROUTE COSTS TO THE ROOT PROCESS:
    int *allBestRouteCosts = new int[npes];
    MPI_Gather(&bestRouteCost, 1, MPI_INT, allBestRouteCosts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // ROOT PROCESS: IDENTIFY WHICH PROCESS SENT THE BEST ROUTE COST:
    int bestRouteCostProcess;
    if (myrank == 0) {
        bestRouteCost = allBestRouteCosts[0];
        for (int i = 1; i < npes; i++) {
            if (allBestRouteCosts[i] < bestRouteCost) {
                bestRouteCost = allBestRouteCosts[i];
                bestRouteCostProcess = i;
            }
        }
        printf("PROCESS %d: The best route cost was found by process %d.\n", myrank, bestRouteCostProcess);
        printf("PROCESS %d: The best route cost is: %d.\n", myrank, bestRouteCost);
    }

    // BROADCAST THE ID OF THE PROCESS THAT FOUND THE BEST ROUTE COST:
    MPI_Bcast(&bestRouteCostProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // MAKE THE PROCESS THAT FOUND THE BEST ROUTE COST SEND ITS BEST ROUTE TO THE ROOT PROCESS:
    if (myrank == bestRouteCostProcess) {
        MPI_Send(bestRoute, xncities, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (myrank == 0) {
        MPI_Recv(bestRoute, xncities, MPI_INT, bestRouteCostProcess, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("PROCESS %d: The best route is: [", myrank);
        for (int i : bestRoute) {
            printf("%d, ", i);
        }
        printf("]\n");
    }

    // GLOBAL TIMER END:
    if (myrank == 0) {
        printf("Total computation took: %f seconds.\n", ((int) ((MPI_Wtime() - start) * 10000) / 10000.0));
    }

    // Finalize MPI:
    MPI_Finalize();

}
