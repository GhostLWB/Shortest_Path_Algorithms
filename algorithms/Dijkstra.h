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

#ifndef SHORTEST_DISTANCE_ALGORITHMS_DIJKSTRA_H
#define SHORTEST_DISTANCE_ALGORITHMS_DIJKSTRA_H
#include <limits>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
using namespace std;

class Dijkstra {
public:
    Dijkstra(const unordered_map<unsigned int, set<Road> >& input_graph);
    double compute_shortest_distance(unsigned int nodeS, unsigned int nodeT);
    double compute_shortest_path(unsigned int nodeS, unsigned int nodeT, vector<unsigned int> &pathRes);
private:
    vector<double> distance;
    vector<bool> visited;
    vector<unsigned int> path;
    unordered_map<unsigned int, set<Road> > graph;
    unsigned int graphSize;

    int find_min_node_id();
};


#endif //SHORTEST_DISTANCE_ALGORITHMS_DIJKSTRA_H
