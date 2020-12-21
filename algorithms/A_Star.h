//
// Created by Dell on 2020/12/20.
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

#ifndef SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
#define SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
#include <set>
#include <unordered_map>
#include <math.h>
#include <queue>
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378137 //地球半径 M
using namespace std;

/**
 * this structure is used for A-star algorithm
 */
class Node_A{
public:
    unsigned int node_id;
    double GScore;// distance
    double FScore;
    Node_A* parent; // parent pointer

    Node_A(unsigned int inputNodeid,double inputGScore, double inputFScore,Node_A * inputParent);
};

class A_Star {
public:
    A_Star(const unordered_map<unsigned int, set<Road> >& input_graph,unordered_map<unsigned int, pair<double,double>>& input_lonlat);
    double ShortestDistance(unsigned int nodeS,unsigned int nodeT);
    double ShortestPath(unsigned int nodeS,unsigned int nodeT);
private:
    unordered_map<unsigned int, set<Road> > graph;
    unordered_map<unsigned int, pair<double,double>> lonlat;

    double rad(double d);
    double haverSin(double x);
    double getDistance(double lat1,double lng1,double lat2,double lng2);
    double compute_f_score(double GScore, unsigned int curNode, unsigned int nodeT);
};


#endif //SHORTEST_DISTANCE_ALGORITHMS_A_STAR_H
