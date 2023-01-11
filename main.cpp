#include <iostream>
#include <mpi.h>

#include "src/Map.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Amount of processes, and rank of the current process:
    int npes, myrank;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // RANK O - PRE-PROCESSING:
    if (myrank==0) {
        // Create a map:
        const string filename = "src/data/distances/dist19.txt";
        Map map = *new Map(filename);
        printf("Process %d created the map.\n", myrank);
    }
    else {
        printf("Process %d is idle.\n", myrank);
    }

    // Finalize MPI:
    MPI_Finalize();
}
