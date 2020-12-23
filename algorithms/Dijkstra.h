//
// Created by Dell on 2020/12/16.
//


#ifndef PQ_ITEM
#define PQ_ITEM
class pq_item{
public:
    unsigned int node_id;
    double toNodeDistance;

    pq_item(unsigned int nodeId, double toNodeDistance);
    bool operator > (const pq_item&n) const{
        return toNodeDistance > n.toNodeDistance;
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
#include <queue>
#include <algorithm>
#include "graph.h"
using namespace std;

class Dijkstra {
public:
    Dijkstra(const unordered_map<unsigned int, vector<Edge> >& input_graph);
    double ShortestDistance(unsigned int nodeS, unsigned int nodeT);
    double ShortestPath(unsigned int nodeS, unsigned int nodeT, vector<unsigned int> &pathRes);

    double ShortestDistancePriorityQueue(unsigned int nodeS, unsigned int nodeT);
    double ShortestPathPriorityQueue(unsigned int nodeS, unsigned int nodeT, vector<unsigned int> &pathRes);
private:
    vector<double> distance;
    vector<bool> visited;
    vector<unsigned int> path;
    unordered_map<unsigned int, vector<Edge> > graph;
    unsigned int graphSize;

    int find_min_node_id();
};


#endif //SHORTEST_DISTANCE_ALGORITHMS_DIJKSTRA_H
