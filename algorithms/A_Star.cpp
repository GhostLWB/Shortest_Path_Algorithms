//
// Created by Dell on 2020/12/20.
//

#include "A_Star.h"

A_Star::A_Star(const unordered_map<unsigned int, vector<Edge>> &input_graph,
               unordered_map<unsigned int, pair<double, double>> &input_lonlat) {
    graph = input_graph;
    lonlat = input_lonlat;
}

Node_A::Node_A(unsigned int inputNodeid, double inputGScore, double inputFScore, Node_A *inputParent) {
    node_id = inputNodeid;
    GScore = inputGScore;
    FScore = inputFScore;
    parent = inputParent;
}

/**
 * Shortest distance algorithm using A star algorithm
 * @param nodeS
 * @param nodeT
 * @return
 */
double A_Star::ShortestDistance(unsigned int nodeS, unsigned int nodeT) {
    set<pair<double, Node_A *>> openlist; // < f_score, node pointer>
    vector<bool> is_in_open = vector<bool>(graph.size(), false);
    vector<bool> is_in_close = vector<bool>(graph.size(), false);

    // init
    double f = compute_f_score(0, nodeS, nodeT);
    Node_A* node = new Node_A(nodeS, 0, f, nullptr);
    openlist.insert(make_pair(f, node));
    is_in_open[nodeS] = true;

    while (!openlist.empty()) {
        pair<double, Node_A *> curr = *openlist.begin();
        openlist.erase(openlist.begin());

        if (curr.second->node_id == nodeT) {
            return curr.second->GScore;
        }

        // move it from openlist to closelist
        is_in_open[curr.second->node_id] = false;
        is_in_close[curr.second->node_id] = true;

        // find its neighbor node and expand
        vector<Edge> roads = graph.find(curr.second->node_id)->second;
        for (auto road:roads) {
            unsigned int neighbor_node_id=road.Neighbor;

            if (is_in_close[neighbor_node_id]) { // neighbor_node is in close list
                continue;
            }

            double GScoreNeigh = curr.second->GScore + road.Road_length;
            double FScoreNeigh = compute_f_score(GScoreNeigh, neighbor_node_id, nodeT);
            Node_A* nodeNeigh = new Node_A(neighbor_node_id, GScoreNeigh, FScoreNeigh, curr.second);

            if (is_in_open[neighbor_node_id]) {
                for (pair<double, Node_A *> iter:openlist) {
                    if (iter.second->node_id == neighbor_node_id and iter.second->GScore >= nodeNeigh->GScore) {
                        // the neighbor node in openlist should be updated
                        openlist.erase(iter);
                        openlist.insert(make_pair(FScoreNeigh,nodeNeigh));
                        break;
                    }
                }
            }else{
                openlist.insert(make_pair(FScoreNeigh, nodeNeigh));
                is_in_open[neighbor_node_id]=true;
            }// end if
        }//end searching neighbor nodes
    }// end while
    return -1; // not found
}


double A_Star::rad(double d) {
    return d * pi / 180.0;
}
double A_Star::haverSin(double x)
{
    double v = sin(x / 2.0);
    return v * v;
}

double A_Star::getDistance(double lon1, double lat1, double lon2, double lat2) {
    double radlon1 = rad(lon1);
    double radlat1 = rad(lat1);
    double radlon2 = rad(lon2);
    double radlat2 = rad(lat2);

    double a = fabs(radlat1 - radlat2);
    double b = fabs(radlon1 - radlon2);

    double h = haverSin(b) + cos(lat1) * cos(lat2) * haverSin(a);
    double distance = 2 * EARTH_RADIUS * asin(sqrt(h));
    return distance;
}

/**
 * The function computing f score, using the Euclidean distance computed by longitude and latitude
 * @param GScore
 * @param curNode
 * @param nodeT
 **/
double A_Star::compute_f_score(double GScore, unsigned int curNode, unsigned int nodeT) {
    //return GScore;
    pair<double, double> lonlatCurNode = lonlat.find(curNode)->second; // longitude,latitude
    pair<double, double> lonlatNodeT = lonlat.find(nodeT)->second;

    double HScore = getDistance(lonlatCurNode.first, lonlatCurNode.second, lonlatNodeT.first, lonlatNodeT.second);
    return HScore + GScore;
}
