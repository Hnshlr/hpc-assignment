cmake_minimum_required(VERSION 3.24)
project (hpc_assignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(hpc_assignment main.cpp src/Person.cpp src/Person.h)

# REQUIRED ADDONS:
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(hpc_assignment ${MPI_C_LIBRARIES})