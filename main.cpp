#include <iostream>
#include <mpi.h>

#include "src/Person.h"

int main(int argc, char *argv[]) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Amount of processes, and rank of the current process:
    int npes, myrank;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // Make each process say 'Hello world!':
    Person person;
    person.say("Hello, I'm a Person person, and I belong to process " + to_string(myrank) + " of " + to_string(npes) + " processes.");


    // Finalize MPI:
    MPI_Finalize();
}
