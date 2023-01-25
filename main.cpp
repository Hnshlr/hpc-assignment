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
    if (myrank == 0) std::cout << "Initialized the MPI environment with " << npes << " processes.\n";

    // SETTINGS:
    std::string distFilename = argv[1];
//     std::string distFilename = "src/data/distances/dist11.txt";

    // GRAPH AND BRANCH-AND-BOUND OBJECTS INITIALIZATION:
    Graph graph = *new Graph(distFilename);
    BNB bnb = *new BNB(graph);
    int ncities = bnb.getNcities();

    // PREFERENCES:
    int path[ncities];              // Path to be computed
    int cost = 0;                   // Cost of the path
    int visited[xncities] = {0};     // Visited nodes
    if (myrank == 0) {              // Root process initializes the path, by selecting a random node to start with
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, ncities - 1);
        path[0] = dis(gen);
        if (argv[2] != nullptr) {
            path[0] = std::stoi(argv[2]);
        }
        visited[path[0]] = 1;
        printf("PROCESS %d: Selected starting node: %d\n", myrank, path[0]);
    }
    // BROADCAST THE STARTING NODE:
    MPI_Bcast(path, ncities, MPI_INT, 0, MPI_COMM_WORLD);

    // FIND EACH PROCESS' STARTING PATHS, DEPENDING ON THE AMOUNT OF PROCESSES AND THE RANK OF THE CURRENT PROCESS:
    std::vector<std::vector<std::vector<int>>> pathsVectors = bnb.getFirstPathsV2(npes, path[0]);
    int totalAmountOfPaths = 0;     // Total amount of paths to be computed
    for (const auto & pathsVector : pathsVectors) {
        totalAmountOfPaths += (int) pathsVector.size();
    }
    int amountOfPaths = (int) pathsVectors[myrank].size();    // Amount of paths to be computed by the current process
    int amountOfNodesPerPath = (int) pathsVectors[myrank][0].size();    // Amount of nodes per path
    int **paths = new int *[amountOfPaths];               // Pointer to the paths to be computed by the current process
    for (int i = 0; i < amountOfPaths; i++) {       // Allocate memory for the paths
        paths[i] = new int[amountOfNodesPerPath];   // Allocate memory for the nodes of each path
        for (int j = 0; j < amountOfNodesPerPath; j++) {    // Fill the paths with the data from the pathsVectors
            paths[i][j] = pathsVectors[myrank][i][j];    // Fill the paths with the data from the pathsVectors
        }
    }

    // TIMER START:
    double start = MPI_Wtime();

    // MAIN JOB: COMPUTE THE SEARCH ON EACH PROCESS:
    if(myrank==0) printf("PROCESS %d: All processes begun their search.\n", myrank);
    for (int i = 0; i < amountOfPaths; i++) {
        cost = 0;
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            visited[paths[i][j]] = 1;
            path[j] = paths[i][j];
            if (j > 0) {
                cost += graph.getDistance(path[j - 1], path[j]);
            }
        }
        // TODO: WORK IN PROGRESS:
        if ((totalAmountOfPaths%npes) == 0 && i<(totalAmountOfPaths/npes)-1
        || (totalAmountOfPaths%npes) != 0 && i<(totalAmountOfPaths/npes)) {
            bnb.searchMPI(path, amountOfNodesPerPath, cost, visited, myrank, npes);
        } else {
            bnb.search(path, amountOfNodesPerPath, cost, visited);
        }
        // TODO: END OF WORK IN PROGRESS.
        for (int j = 0; j < amountOfNodesPerPath; j++) {
            visited[paths[i][j]] = 0;
        }
    }
    if(myrank==0) printf("PROCESS %d: All processes finished their search.\n", myrank);
    int bestRouteCost = bnb.getBestRouteCost();
    int bestRoute[ncities];
    for (int i = 0; i < ncities; i++) {
        bestRoute[i] = bnb.getBestRoute()[i];
    }

    // MAKE ALL PROCESSES SEND THEIR BEST ROUTE COSTS TO THE ROOT PROCESS:
    int *allBestRouteCosts = new int[npes];
    MPI_Gather(&bestRouteCost, 1, MPI_INT, allBestRouteCosts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // ROOT PROCESS: IDENTIFY WHICH PROCESS SENT THE BEST ROUTE COST:
    int bestRouteCostProcess;
    if (myrank == 0) {
        bestRouteCost = allBestRouteCosts[0];
        bestRouteCostProcess = 0;
        for (int i = 1; i < npes; i++) {
            if (allBestRouteCosts[i] < bestRouteCost) {
                bestRouteCost = allBestRouteCosts[i];
                bestRouteCostProcess = i;
            }
        }
        printf("PROCESS %d: The best route cost is: %d.\n", myrank, bestRouteCost);
    }

    // BROADCAST THE BEST ROUTE COST PROCESS:
    MPI_Bcast(&bestRouteCostProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // MAKE THE PROCESS THAT FOUND THE BEST ROUTE COST BROADCAST ITS BEST ROUTE TO ALL PROCESSES:
    MPI_Bcast(bestRoute, ncities, MPI_INT, bestRouteCostProcess, MPI_COMM_WORLD);

    // MAKE THE PROCESS 0 ANNOUNCE THE BEST ROUTE:
    if (myrank == 0) {
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

    // FINALIZE THE MPI ENVIRONMENT:
    MPI_Finalize();

}
