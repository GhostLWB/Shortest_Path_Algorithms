//
// Created by Dell on 2020/12/16.
//

#ifndef ROAD_STRUCTURE
#define ROAD_STRUCTURE
class Edge {
public:
    unsigned int Neighbor;
    double Road_length;

    Edge() = default;
    Edge(unsigned int neighbor, double roadLength) : Neighbor(neighbor), Road_length(roadLength) {}
private :
    bool operator<(const Edge &a) const {
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

// function declaration
void read_file(string file_path,unordered_map<unsigned int, vector<Edge> > & graph);
void read_lonlat(string file_path,unordered_map<unsigned int, pair<double,double>> &lonlat);
#endif //SHORTEST_DISTANCE_ALGORITHMS_GRAPH_H
