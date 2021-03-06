//
// Created by Dell on 2020/12/16.
//

#include "Dijkstra.h"
/**
 *  constructor for class graph.
 *  assign graph size for variable graphSize
 */
Dijkstra::Dijkstra(Graph* input_graph){
    graph=input_graph;
};
/**
 * find the currently minimum distance node by traversing
 * @param distance
 * @param visited
 * @param graphSize
 * @return
 */
int Dijkstra::find_min_node_id() {
    double min = numeric_limits<float>::max();
    int minIndex = -1;
    for (int i = 0; i < this->graph->graph_size; i++) {
        if (visited[i] == false && distance[i] < min) {
            min = distance[i];
            minIndex = i;
        }
    }
    return minIndex;
}
/**
 * compute the shortest distance using classic Dijktra
 * @param nodeS
 * @param nodeT
 * @return
 */
double Dijkstra::ShortestDistance(unsigned int nodeS, unsigned int nodeT) {
    // init
    distance=vector<double>(this->graph->graph_size,numeric_limits<float>::max());
    visited=vector<bool>(this->graph->graph_size,false);
    //path=vector<unsigned int> (graphSize,-1);


    distance[nodeS]=0.0;
    // now start iterating, calculate distance from reference node to other nodes
    for (int k = 0; k <= this->graph->graph_size; k++) {
        // find the min distance node that is unvisited
        int currNode = find_min_node_id();
        if (currNode == -1) { // the rest nodes are unreachable
            break;
        }
        vector<Edge*> adjcent = this->graph->graph_nodes.find(currNode)->second->edges;
        for (Edge* iterator: adjcent) {
            unsigned int adjNode = iterator->node_id;
            // update distance array
            if (distance[currNode] + iterator->distance < distance[adjNode]) {
                distance[adjNode] = distance[currNode] + iterator->distance;
            }
        }

        visited[currNode] = true;
    }
    return distance[nodeT];
}
/**
 * compute the shortest distance and record the shortest path using classic Dijktra
 * @param nodeS
 * @param nodeT
 * @param pathRes the result path stores here
 * @return
 */
double Dijkstra::ShortestPath(unsigned int nodeS, unsigned int nodeT, vector<unsigned int> &pathRes) {
    // init
    distance=vector<double>(this->graph->graph_size,numeric_limits<float>::max());
    visited=vector<bool>(this->graph->graph_size,false);
    path=vector<unsigned int> (this->graph->graph_size,-1);
    distance[nodeS]=0.0;
    // now start iterating, calculate distance from reference node to other nodes
    for (int k = 0; k <= this->graph->graph_size; k++) {
        // find the min distance node that is unvisited
        int currNode = find_min_node_id();
        if (currNode == -1) { // the rest nodes are unreachable
            break;
        }
        vector<Edge*> adjcent = this->graph->graph_nodes.find(currNode)->second->edges;
        for (Edge* iterator: adjcent) {
            unsigned int adjNode = iterator->node_id;
            // update distance array
            if (distance[currNode] + iterator->distance < distance[adjNode]) {
                distance[adjNode] = distance[currNode] + iterator->distance;
                path[adjNode]=currNode;
            }
        }

        visited[currNode] = true;
    }
    // find the path stored in path vector
    stack<unsigned int> q;
    unsigned int n=nodeT;
    while(path[n]!=-1){
        q.push(n);
        n=path[n];
    }
    pathRes.push_back(nodeS);
    while(!q.empty()){
        pathRes.push_back(q.top());
        q.pop();
    }
    return distance[nodeT];
}

/**
 * compute the shortest distance using priority queue optimized Dijktra
 * @param nodeS
 * @param nodeT
 * @return
 */
double Dijkstra::ShortestDistancePriorityQueue(unsigned int nodeS, unsigned int nodeT) {
    // init
    distance=vector<double>(this->graph->graph_size,numeric_limits<float>::max());
    priority_queue<pq_item,vector<pq_item>,greater<pq_item>> myQueue;

    distance[nodeS]=0.0;
    myQueue.push({nodeS,distance[nodeS]});
    // now start iterating, calculate distance from reference node to other nodes
    while(!myQueue.empty()) {
        // find the min distance node that is unvisited
        pq_item currItem=myQueue.top();
        myQueue.pop();
        int currNode = currItem.node_id;
        if (distance[currNode] < currItem.toNodeDistance) {
            continue;
        }
        vector<Edge*> adjcent = this->graph->graph_nodes.find(currNode)->second->edges;
        for (Edge* iterator: adjcent) {
            unsigned int adjNode = iterator->node_id;
            // update distance array
            if (  distance[currNode] + iterator->distance < distance[adjNode]) {
                distance[adjNode] = distance[currNode] + iterator->distance;
                myQueue.push({adjNode,distance[adjNode]});
            }
        }

    }
    return distance[nodeT];
}

/**
 * compute the shortest distance and record the shortest path using priority queue optimized Dijktra
 * @param nodeS
 * @param nodeT
 * @param pathRes
 * @return
 */
double Dijkstra::ShortestPathPriorityQueue(unsigned int nodeS, unsigned int nodeT,
                                           vector<unsigned int> &pathRes) {
    // init
    distance=vector<double>(this->graph->graph_size,numeric_limits<float>::max());
    priority_queue<pq_item,vector<pq_item>,greater<pq_item>> myQueue;
    path=vector<unsigned int> (this->graph->graph_size,-1);

    distance[nodeS]=0.0;
    myQueue.push({nodeS,distance[nodeS]});
    // now start iterating, calculate distance from reference node to other nodes
    while(!myQueue.empty()) {
        // find the min distance node that is unvisited
        pq_item currItem=myQueue.top();
        myQueue.pop();
        int currNode = currItem.node_id;
        if (distance[currNode] < currItem.toNodeDistance) {
            continue;
        }
        vector<Edge*> adjcent = this->graph->graph_nodes.find(currNode)->second->edges;
        for (Edge* iterator: adjcent) {
            unsigned int adjNode = iterator->node_id;
            // update distance array
            if (  distance[currNode] + iterator->distance < distance[adjNode]) {
                distance[adjNode] = distance[currNode] + iterator->distance;
                myQueue.push({adjNode,distance[adjNode]});
                path[adjNode]=currNode;
            }
        }

    }
    // find the path stored in path vector
    stack<unsigned int> q;
    unsigned int n=nodeT;
    while(path[n]!=-1){
        q.push(n);
        n=path[n];
    }
    pathRes.push_back(nodeS);
    while(!q.empty()){
        pathRes.push_back(q.top());
        q.pop();
    }
    return distance[nodeT];
}
pq_item::pq_item(unsigned int nodeId, double toNodeDistance) : node_id(nodeId), toNodeDistance(toNodeDistance) {}
