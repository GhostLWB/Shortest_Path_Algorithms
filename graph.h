//
// Created by Dell on 2020/12/16.
//

#ifndef ROAD_STRUCTURE
#define ROAD_STRUCTURE
class Road {
public:
    unsigned int Start_NID;
    unsigned int End_NID;
    double Road_length;

public:
    Road() = default;

    Road(unsigned int S_NID, unsigned int E_NID, double RL) : \
    Start_NID(S_NID), End_NID(E_NID), Road_length(RL) {};

    bool operator<(const Road &a) const {
        return a.Road_length <= Road_length;
    }
};
#endif

#ifndef SHORTEST_DISTANCE_ALGORITHMS_GRAPH_H
#define SHORTEST_DISTANCE_ALGORITHMS_GRAPH_H
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

extern unordered_map<unsigned int, set<Road> > graph;

void read_file(string file_path);
#endif //SHORTEST_DISTANCE_ALGORITHMS_GRAPH_H
