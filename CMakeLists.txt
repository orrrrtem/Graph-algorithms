cmake_minimum_required(VERSION 3.12)
project(bridges)

set(CMAKE_CXX_STANDARD 17)

add_executable(bridges main.cpp graph.h algorithms.h benchmark.h sorts.h tests.h reader.h FL_SSSP.h benchmark_shortest_paths.h)