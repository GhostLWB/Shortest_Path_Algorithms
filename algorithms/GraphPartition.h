//
// Created by liwenbin on 2021/01/03
// 

#ifndef GRAPHPARTITION_H
#define GRAPHPARTITION_H

// include STL section
#include<vector>
#include<set>
#include<queue>
#include<random>
#include<algorithm>
// include local hearder section
#include "Graph.h"

using namespace std;

// class declare section
class MultiNode {
public:
    unsigned int mnode_id;
    vector<unsigned int> nodes;
    vector<unsigned int> edges;
    set<int> category;
    vector<MultiNode *> prevMultiNodes;

    MultiNode(unsigned int input_mnode_id, vector<unsigned int> &input_nodes, vector<unsigned int> &input_edges,
              set<int> &input_category);

    MultiNode();
};


class CoarsenGraph {
public:
    vector<int> Match;
    vector<int> Map;
    vector<int> Map_origin;
    vector<MultiNode *> Nodes;

    CoarsenGraph(vector<int> &input_Match, vector<int> &input_Map, vector<MultiNode *> &input_Nodes);

    CoarsenGraph(CoarsenGraph *prevCoarsenGraph);

    CoarsenGraph(int nodeNum);

    ~CoarsenGraph();
};

// function declare section
void coarsening(Graph *graph, vector<CoarsenGraph *> &coarsening_stage);

MultiNode *
matching(unsigned int partitionID, vector<MultiNode *> &matchedMultiNodePrevStage, CoarsenGraph *currCoarsenGraph);

void linearMatch(MultiNode *pivotNode, CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                 vector<unsigned int> &res);

void denseMatch(MultiNode *pivotNode, CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                vector<unsigned int> &res);
void minimunBorderMatch(MultiNode *pivotNode, Graph *graph,CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                        vector<unsigned int> &res);
void randomMatch(MultiNode *pivotNode,  CoarsenGraph *currCoarsenGraph,vector<unsigned int> &res);
#endif
