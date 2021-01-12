//
// Created by liwenbin on 2021/01/01
//

#ifndef GRAPH_H
#define GRAPH_H

#include<unordered_map>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iostream>
#include<ctime>
#include "configure.h"

using namespace std;

class Edge{
public:
	unsigned int node_id;
	double distance;
	Edge(unsigned int input_node_id,double input_distance);
};


class Node{
public:
	unsigned int node_id;
	int category;
	pair<double,double> lonlat; // first=longitude second=latitude
	vector<Edge*> edges;
	
	Node(unsigned int input_node_id);
	Node(unsigned int input_node_id,int input_category);
};
class Graph{
public:
	unordered_map<unsigned int, Node*> graph_nodes;
	unsigned int graph_size;

	Graph();
	~Graph();
};


#endif
