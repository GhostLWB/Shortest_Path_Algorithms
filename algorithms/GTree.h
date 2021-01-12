//
// Created by liwenbin on 2021/01/06
//

#ifndef GTREE_H
#define GTREE_H
// STL include section
#include<vector>
#include<set>
#include<string>
#include<map>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<float.h>
using namespace std;

// local include section
#include "GraphPartition.h"
#include "Graph.h"
#include "A_Star.h"

// class declare section
class GNode{
public:
	vector<unsigned int> border;
	set<int> categories;
	map<string,double> distMatrix;
	vector<GNode*> childGNode;

	// function declaration
    GNode();
};


class GTree{
public:
	vector<GNode*> leaf;
	GNode* root;
	map<string,double> computedDistance;

	GTree(vector<CoarsenGraph*> &stages,Graph *graph);
	double shortestDistance(unsigned int nodeA,unsigned int nodeB,Graph *graph);

private:
    GNode* findCommonAncestor(GNode *root,GNode *nodeA,GNode *nodeB);
    GNode* buildGNode(MultiNode* MNode,vector<CoarsenGraph*> &stages,int coarseGraphIndex,Graph* graph,vector<GNode*> &leaf);
    bool containsGNode(GNode *root,GNode *targetGNode);
	void saveDistanceToFile(GNode *gnode);
	void loadDistanceFromFile();
};


#endif
