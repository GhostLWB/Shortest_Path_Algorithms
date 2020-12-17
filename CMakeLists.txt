cmake_minimum_required(VERSION 3.17)
project(Shortest_Distance_Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Shortest_Distance_Algorithms main.cpp algorithms/Dijkstra.cpp algorithms/Dijkstra.h graph.cpp graph.h)