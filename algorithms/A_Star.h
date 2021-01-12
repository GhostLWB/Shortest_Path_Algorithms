//
// Created by Dell on 2020/12/20.
//

#ifndef SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
#define SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
#include <set>
#include <unordered_map>
#include <math.h>
#include <queue>
#include <memory>
#include "graph.h"
#include "algorithm"
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半径 KM
using namespace std;

/**
 * this structure is used for A-star algorithm
 */
class Node_A{
public:
    unsigned int node_id;
    double GScore;// distance
    double FScore;
    shared_ptr<Node_A> parent; // parent pointer

    Node_A(unsigned int inputNodeid,double inputGScore, double inputFScore,shared_ptr<Node_A> inputParent);
};

class A_Star {
public:
    A_Star(const unordered_map<unsigned int, vector<Edge> >& input_graph, unordered_map<unsigned int, pair<double,double>>& input_lonlat);
    double ShortestDistance(unsigned int nodeS,unsigned int nodeT);
    double ShortestPath(unsigned int nodeS,unsigned int nodeT,vector<unsigned int> & pathRes);
private:
    unordered_map<unsigned int, vector<Edge> > graph;
    unordered_map<unsigned int, pair<double,double>> lonlat;

    double rad(double d);
    double haverSin(double x);
    double getDistance(double lat1,double lng1,double lat2,double lng2);
    double compute_f_score(double GScore, unsigned int curNode, unsigned int nodeT);
};


#endif //SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
