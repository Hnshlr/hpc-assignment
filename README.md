# HIGH PERFORMANCE TECHNICAL COMPUTING - ASSIGNMENT

© Copyright 2022, All rights reserved to Hans Haller, CSTE-CIDA Student at Cranfield Uni. SATM, Cranfield, UK.

Author's Github: https://www.github.com/Hnshlr

### Implementation of a combinatorial optimization problem using distributed-memory parallel programming techniques.

### - How to compile:
Make sure you use intel loaded mpicxx compiler. Then run the following command:
``` mpicxx main.cpp src/Graph.cpp src/Graph.h src/BNB.h src/BNB.cpp``` in the root directory of the project.

### - How to run:
To run the program, the path of the distances.txt file must be inputed as a command line argument.
You may use the following command:
``` mpirun -n <number of processes> ./a.out <distances file path>```

### - Submission files:
Examples of .sub files can be found in /queues. Examples of executions of the program can be found in /jobs.