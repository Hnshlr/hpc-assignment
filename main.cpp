#include <iostream>
#include <mpi.h>

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

int main(int argc, char *argv[]) {

    // TIMER START:
    double start = MPI_Wtime();
    std::string distFilename = "src/data/distances/dist11.txt";

    // MAIN:
    BNB bnb = *new BNB(Graph(distFilename));

    // COMPUTE THE SEARCH ON EACH PROCESS:
    std::vector<int> initialPath = {};
    bnb.search(initialPath);
    bnb.bestRouteToString();
    bnb.bestCostToString();

    // INDIVIDUAL TIMER END:
    std::cout << "Computation took: " << (int) ((MPI_Wtime() - start) * 100) / 100.0 << " seconds.\n" << std::endl;
}
