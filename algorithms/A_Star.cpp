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
    Node_A *node = new Node_A(nodeS, 0, f, nullptr);
    openlist.insert(make_pair(f, node));
    is_in_open[nodeS] = true;

    while (!openlist.empty()) {
        // fetch the node with the smallest FScore in openlist
        pair<double, Node_A *> curr = *openlist.begin();

        if (curr.second->node_id == nodeT) {
            return curr.second->GScore;
        }

        // move it from openlist to closelist
        is_in_open[curr.second->node_id] = false;
        openlist.erase(openlist.begin());
        is_in_close[curr.second->node_id] = true;

        // find its neighbor node and expand
        vector<Edge> roads = graph.find(curr.second->node_id)->second;
        for (auto road:roads) {
            unsigned int neighbor_node_id = road.Neighbor;

            if (is_in_close[neighbor_node_id]) { // neighbor_node is in close list
                continue;
            }

            double GScoreNeigh = curr.second->GScore + road.Road_length;
            double FScoreNeigh = compute_f_score(GScoreNeigh, neighbor_node_id, nodeT);
            Node_A *nodeNeigh = new Node_A(neighbor_node_id, GScoreNeigh, FScoreNeigh, curr.second);

            if (is_in_open[neighbor_node_id]) {
                for (pair<double, Node_A *> iter:openlist) {
                    if (iter.second->node_id == neighbor_node_id and iter.second->GScore >= nodeNeigh->GScore) {
                        // the neighbor node in openlist should be updated
                        openlist.erase(iter);
                        openlist.insert(make_pair(FScoreNeigh, nodeNeigh));
                        break;
                    }
                }
            } else {
                openlist.insert(make_pair(FScoreNeigh, nodeNeigh));
                is_in_open[neighbor_node_id] = true;
            }// end if
        }//end searching neighbor nodes
    }// end while
    return -1; // not found
}

/**
 * Shortest path algorithm using A star algorithm
 * @param nodeS
 * @param nodeT
 * @param pathRes
 * @return
 */
double A_Star::ShortestPath(unsigned int nodeS, unsigned int nodeT, vector<unsigned int> &pathRes) {
    set<pair<double, Node_A *>> openlist; // < f_score, node pointer>
    vector<bool> is_in_open = vector<bool>(graph.size(), false);
    vector<bool> is_in_close = vector<bool>(graph.size(), false);

    // init
    double f = compute_f_score(0, nodeS, nodeT);
    Node_A *node = new Node_A(nodeS, 0, f, nullptr);
    openlist.insert(make_pair(f, node));
    is_in_open[nodeS] = true;

    while (!openlist.empty()) {
        // fetch the node with the smallest FScore in openlist
        pair<double, Node_A *> curr = *openlist.begin();

        if (curr.second->node_id == nodeT) {
            Node_A *node_ptr = curr.second;
            // find the path in a reversive manner
            while (node_ptr != nullptr) {
                pathRes.push_back(node_ptr->node_id);
                node_ptr = node_ptr->parent;
            }
            reverse(pathRes.begin(), pathRes.end());
            return curr.second->GScore;
        }

        // move it from openlist to closelist
        is_in_open[curr.second->node_id] = false;
        openlist.erase(openlist.begin());
        is_in_close[curr.second->node_id] = true;

        // find its neighbor node and expand
        vector<Edge> roads = graph.find(curr.second->node_id)->second;
        for (auto road:roads) {
            unsigned int neighbor_node_id = road.Neighbor;

            if (is_in_close[neighbor_node_id]) { // neighbor_node is in close list
                continue;
            }

            double GScoreNeigh = curr.second->GScore + road.Road_length;
            double FScoreNeigh = compute_f_score(GScoreNeigh, neighbor_node_id, nodeT);
            Node_A *nodeNeigh = new Node_A(neighbor_node_id, GScoreNeigh, FScoreNeigh, curr.second);

            if (is_in_open[neighbor_node_id]) {
                for (pair<double, Node_A *> iter:openlist) {
                    if (iter.second->node_id == neighbor_node_id and iter.second->GScore > nodeNeigh->GScore) {
                        //the neighbor node in openlist should be updated
                        openlist.erase(iter);
                        openlist.insert(make_pair(FScoreNeigh,nodeNeigh));
                        break;
                    }
                }
            } else {
                openlist.insert(make_pair(FScoreNeigh, nodeNeigh));
                is_in_open[neighbor_node_id] = true;
            }// end if
        }//end searching neighbor nodes
    }// end while
    return -1; // not found
}

double A_Star::rad(double d) {
    return d * pi / 180.0;
}

double A_Star::haverSin(double x) {
    double v = sin(x / 2.0);
    return v * v;
}

double A_Star::getDistance(double lon1, double lat1, double lon2, double lat2) {
    double a;
    double b;
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    a = radLat1 - radLat2;
    b = rad(lon1) - rad(lon2);
    double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
    s = s * EARTH_RADIUS;
    s = s * 1000;
    return s;
}

/**
 * The function computing f score, using the Euclidean distance computed by longitude and latitude
 * but maybe it is not an admissible heuristic, so i just use the plane distance instead.
 * @param GScore
 * @param curNode
 * @param nodeT
 **/
double A_Star::compute_f_score(double GScore, unsigned int curNode, unsigned int nodeT) {
    //return GScore;
    pair<double, double> lonlatCurNode = lonlat.find(curNode)->second; // longitude,latitude
    pair<double, double> lonlatNodeT = lonlat.find(nodeT)->second;

    double HScore;
    // latitude and longitude distance, but doesnt work
    //HScore= getDistance(lonlatCurNode.first, lonlatCurNode.second, lonlatNodeT.first, lonlatNodeT.second);
    // plane distance, works
    HScore=sqrt(pow((lonlatCurNode.first-lonlatNodeT.first),2)+pow((lonlatCurNode.second-lonlatNodeT.second),2));
    return HScore + GScore;
}
