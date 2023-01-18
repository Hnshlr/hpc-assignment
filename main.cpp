#include <iostream>
#include <mpi.h>
#include <random>

#include "src/Graph.h"
#include "src/BNB.h"

// 17 villes, 32 processes= 220 secondes.

//int main(int argc, char *argv[]) {
//
//    // INITIALIZE THE MPI ENVIRONMENT:
//    MPI_Init(&argc, &argv);
//    // AMOUNT OF PROCESSES, AND RANK OF THE CURRENT PROCESS:
//    int npes, myrank;
//    MPI_Comm_size(MPI_COMM_WORLD, &npes);
//    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
//    // TIMER START:
//    double start = MPI_Wtime();
//
//    // SETTINGS:
//    // std::string distFilename = argv[1];
//    std::string distFilename = "src/data/distances/dist11.txt";
//
//    // MAIN:
//    BNB bnb = *new BNB(Graph(distFilename));
//    std::vector<std::vector<int>> bestRoutes;
//    std::vector<int> globalBestRoute;
//
//    // FIND THE STARTING PATH DEPENDING ON THE AMOUNT OF PROCESSES AND THE RANK OF THE CURRENT PROCESS:
//    std::vector<std::vector<std::vector<int>>> paths = bnb.getFirstPaths(npes);
//    std::vector<std::vector<int>> myPaths = paths[myrank];
//    std::cout << "PROCESS " << myrank << ": Now working on " << myPaths.size() << " paths: [";
//    for (auto & path : myPaths) {
//        std::cout << "[";
//        for (auto & node : path) {
//            std::cout << node << ", ";
//        }
//        std::cout << "], ";
//    }
//    std::cout << "]" << std::endl;
//
//    // COMPUTE THE SEARCH ON EACH PROCESS:
//    for (auto & path : paths[myrank]) {
//        bnb.search(path);
//    }
//    std::cout << "PROCESS " << myrank << ": " << std::endl;
//    bnb.bestRouteToString();
//    bnb.bestCostToString();
//
//    // INDIVIDUAL TIMER END:
//    std::cout << "Computation took: " << (int) ((MPI_Wtime() - start) * 100) / 100.0 << " seconds.\n" << std::endl;
//
//    // GATHER THE BEST ROUTES OF EACH PROCESS:
//    int *globalBuffer = new int[bnb.getGraph().getDistances().size() * npes];
//    MPI_Gather(bnb.getBestRoute().data(), (int) bnb.getGraph().getDistances().size(), MPI_INT, globalBuffer, (int) bnb.getGraph().getDistances().size(), MPI_INT, 0, MPI_COMM_WORLD);
//    MPI_Barrier(MPI_COMM_WORLD);
//
//
//    // FIND AND PRINT THE GLOBAL BEST ROUTE:
//    if (myrank == 0) {
//        // FILL THE BESTROUTES VECTOR WITH THE GLOBALBUFFER DATA:
//        for (int i = 0; i < npes; i++) {
//            std::vector<int> route;
//            for (int j = 0; j < bnb.getGraph().getDistances().size(); j++) {
//                route.push_back(globalBuffer[i * bnb.getGraph().getDistances().size() + j]);
//            }
//            bestRoutes.push_back(route);
//        }
//        // COMPARE ROUTES TO FIND THE GLOBAL BEST ROUTE:
//        for (auto & route : bestRoutes) {
//            if (bnb.isRouteBetter(route)) {
//                globalBestRoute = route;
//            }
//        }
//        std::cout << "PROCESS " << myrank << ": " << std::endl;
//        bnb.setBestRoute(globalBestRoute);
//        bnb.bestRouteToString();
//        bnb.bestCostToString();
//        // GLOBAL TIMER END:
//        std::cout << "Total Computation took: " << (int) ((MPI_Wtime() - start) * 100) / 100.0 << " seconds.\n" << std::endl;
//    }
//
//    // Finalize MPI:
//    MPI_Finalize();
//}

#define ncities 17

int main(int argc, char *argv[]) {

    // INITIALIZE THE MPI ENVIRONMENT:
    MPI_Init(&argc, &argv);
    // AMOUNT OF PROCESSES, AND RANK OF THE CURRENT PROCESS:
    int npes, myrank;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // SETTINGS:
    // std::string distFilename = argv[1];
    std::string distFilename = "src/data/distances/dist17.txt";

    // MAIN:
    Graph graph = *new Graph(distFilename);
    BNB bnb = *new BNB(graph);


    // PREFERENCES:
    int path[ncities];              // Path to be computed
    int cost = 0;                   // Cost of the path
    int visited[ncities] = {0};     // Visited nodes
    if (myrank == 0) {              // Root process initializes the path, by selecting a random node to start with
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, ncities - 1);
        path[0] = dis(gen);
        visited[path[0]] = 1;
        printf("PROCESS %d: Selected starting node: %d\n", myrank, path[0]);
    }
    // BROADCAST THE STARTING NODE:
    MPI_Bcast(path, ncities, MPI_INT, 0, MPI_COMM_WORLD);

    // FIND EACH PROCESS' STARTING PATHS, DEPENDING ON THE AMOUNT OF PROCESSES AND THE RANK OF THE CURRENT PROCESS:
    std::vector<std::vector<std::vector<int>>> pathsVectors = bnb.getFirstPaths(32, path[0]);
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

    // INDIVIDUAL TIMER END:
    printf("Computation took: %f seconds.\n", ((int) ((MPI_Wtime() - start) * 10000) / 10000.0));

    // Finalize MPI:
    MPI_Finalize();
}
