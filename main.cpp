#include <iostream>
#include <mpi.h>

#include "src/Graph.h"
#include "src/BNB.h"

using namespace std;
// 17 villes, 32 processes= 220 secondes.


//int main(int argc, char *argv[]) {
//    // TIMER START:
//    double start = MPI_Wtime();
//
//    // MAIN:
//    const string distFilename = "src/data/distances/dist11.txt";
//    Graph graph = *new Graph(distFilename);
//    BNB bnb = *new BNB(graph);
//    bnb.search(vector<int>{});
//    bnb.bestRouteToString();
//    bnb.bestCostToString();
//
//    // TIMER END:
//    double end = MPI_Wtime();
//    cout << "Computation took: " << (int) ((end - start) * 100) / 100.0 << " seconds" << endl;
//}


int main(int argc, char *argv[]) {

    // INITIALIZE THE MPI ENVIRONMENT:
    MPI_Init(&argc, &argv);
    // AMOUNT OF PROCESSES, AND RANK OF THE CURRENT PROCESS:
    int npes, myrank;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // TIMER START:
    double start = MPI_Wtime();


    // MAIN:
    Graph graph;
    BNB bnb;

    if(myrank==0) {
        const string distFilename = "src/data/distances/dist11.txt";
        graph = *new Graph(distFilename);
        bnb = *new BNB(graph);
    }
    // SEND THE GRAPH TO ALL PROCESSES:
    MPI_Bcast(&graph, sizeof(graph), MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&bnb, sizeof(bnb), MPI_BYTE, 0, MPI_COMM_WORLD);

    cout << "Process " << myrank << " received the the BNB object, and the graph object. "
                                    "[Graph size: " << graph.getDistances().size() << ", " <<
                                    "BNB size: " << bnb.getNodes().size() << "]" << endl;

    // COMPUTE THE SEARCH ON EACH PROCESS:
    bnb.search(vector<int>{});
    bnb.bestRouteToString();
    bnb.bestCostToString();

    // TIMER END:
    double end = MPI_Wtime();
    cout << "Computation took: " << (int) ((end - start) * 100) / 100.0 << " seconds" << endl;

    // Finalize MPI:
    MPI_Finalize();
}

