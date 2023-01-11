#include <iostream>
#include <mpi.h>

#include "src/Map.h"

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
        cout << "Process " << myrank << " created the map." << endl;
    }
    else {
        cout << "Process " << myrank << " is idle." << endl;
    }

    // Finalize MPI:
    MPI_Finalize();
}
