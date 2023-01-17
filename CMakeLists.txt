cmake_minimum_required(VERSION 3.24)
project(hpc_assignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(hpc_assignment main.cpp src/Graph.cpp src/Graph.h src/BNB.h src/BNB.cpp)

add_custom_command(TARGET hpc_assignment POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/data $<TARGET_FILE_DIR:hpc_assignment>/src/data)

# REQUIRED ADDONS:
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(hpc_assignment ${MPI_C_LIBRARIES})