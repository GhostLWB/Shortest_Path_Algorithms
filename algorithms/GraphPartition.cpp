#include "GraphPartition.h"

MultiNode::MultiNode() {
    this->mnode_id = 0;
    this->nodes = vector<unsigned int>();
    this->edges = vector<unsigned int>();
    this->category = set<int>();
    this->prevMultiNodes = vector<MultiNode *>();
}

MultiNode::MultiNode(unsigned int input_mnode_id, vector<unsigned int> &input_nodes, vector<unsigned int> &input_edges,
                     set<int> &input_category) {
    this->mnode_id = input_mnode_id;
    this->nodes = input_nodes;
    this->edges = input_edges;
    this->category = input_category;
    this->prevMultiNodes = vector<MultiNode *>();
}

CoarsenGraph::CoarsenGraph(vector<int> &input_Match, vector<int> &input_Map, vector<MultiNode *> &input_Nodes) {
    this->Match = input_Match;
    this->Map = input_Map;
    this->Nodes = input_Nodes;
}

CoarsenGraph::CoarsenGraph(int nodeNum) {
    this->Match = vector<int>(nodeNum, -1);
    this->Map = vector<int>(nodeNum, -1);
    this->Nodes = vector<MultiNode *>();
}

CoarsenGraph::CoarsenGraph(CoarsenGraph *prevCoarsenGraph) {
    int prevNodeSize = prevCoarsenGraph->Nodes.size();
    this->Match = vector<int>(prevNodeSize, -1);
    this->Map = vector<int>(prevNodeSize, -1);
    this->Map_origin = vector<int>(prevCoarsenGraph->Map_origin.size(), -1);
    this->Nodes = vector<MultiNode *>();
}

CoarsenGraph::~CoarsenGraph() {
    for (MultiNode *multinodePtr:this->Nodes) {
        delete multinodePtr;
    }
}

/**
 *
 * @param pivotNode
 * @param graph
 * @param currCoarsenGraph
 * @param res
 */
void linearMatch(MultiNode *pivotNode, CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                 vector<unsigned int> &res) {
    queue<MultiNode *> exploreQ;
    set<unsigned int> visited;

    exploreQ.push(pivotNode);
    while (!exploreQ.empty()) {
        MultiNode *currNode = exploreQ.front();
        exploreQ.pop();
        visited.insert(currNode->mnode_id);
        res.push_back(currNode->mnode_id);

        // 判断currNode的相邻结点是否属于线性图
        for (unsigned int adjMNode_id:currNode->edges) {
            if (visited.count(adjMNode_id) != 0 or
                currCoarsenGraph->Map[adjMNode_id] != -1) {// 这个相邻结点是被访问过的，或者已经被match过的，就pass
                continue;
            } else {
                // 查找adjMNode_id的度
                if (prevCoarsenGraph->Nodes[adjMNode_id]->edges.size() == 2) {// 说明neighborNode也是线性图的一部分
                    exploreQ.push(prevCoarsenGraph->Nodes[adjMNode_id]);
                }// end if
            }// end else
        } // end for traversing currNode's neighbor
    } // end while
}// end function

/**
 *
 * @param pivotNode
 * @param graph
 * @param currCoarsenGraph
 * @param res
 */
void denseMatch(MultiNode *pivotNode, CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                vector<unsigned int> &res) {
    vector<unsigned int> candidateVec = vector<unsigned int>();// stores partition id
    candidateVec.push_back(pivotNode->mnode_id);
    // 寻找pivotNode的所有未match的结点
    for (unsigned int adjMultiNode:pivotNode->edges) {
        if (currCoarsenGraph->Map[adjMultiNode] == -1) {
            candidateVec.push_back(adjMultiNode);
        }
    }
    // 对于candidateVec中的每个candidate，计算它的未match的邻接结点的数量（度），寻找度最大的一个作为pivotNode
    int maxDegree = 0;
    unsigned int maxDegreeCandidate = 0;
    for (unsigned int mnode_id:candidateVec) {
        // 计算它的未match的邻接结点的数量
        int count = 0;
        for (unsigned int adjMnode:prevCoarsenGraph->Nodes[mnode_id]->edges) {
            if (currCoarsenGraph->Map[adjMnode] == -1) count++;
        }
        if (count >= maxDegree) {
            maxDegree = count;
            maxDegreeCandidate = mnode_id;
        }
    }
    // 将这个pivotNode的所有未match的node加入match集合（res）
    set<unsigned int> resSet;
    resSet.insert(maxDegreeCandidate);
    vector<unsigned int> mulNodeEdges = prevCoarsenGraph->Nodes[maxDegreeCandidate]->edges;
    for (unsigned int adjMnode:mulNodeEdges) {
        if (currCoarsenGraph->Map[adjMnode] == -1) resSet.insert(adjMnode);
    }
    for (unsigned int i:resSet) res.push_back(i);
}

void
minimunBorderMatch(MultiNode *pivotNode, Graph *graph, CoarsenGraph *prevCoarsenGraph, CoarsenGraph *currCoarsenGraph,
                   vector<unsigned int> &res) {
    // 寻找pivotNode所有未match的邻居
    vector<unsigned int> neighVec = vector<unsigned int>();
    neighVec.push_back(pivotNode->mnode_id);
    for (unsigned int neiNodeID:pivotNode->edges) {
        if (currCoarsenGraph->Map[neiNodeID] == -1) neighVec.push_back(neiNodeID);
    }

    // 对于每个邻居，计算如果用它进行match将会产生多少border (vertex级别的)
    int minBorderNum = 0x7fffffff;
    unsigned int minBorderNodeID = 0;
    for (unsigned int candidate:neighVec) {
        // 使用candidate进行match的话
        int currborder = 0;
        set<unsigned int> visited = set<unsigned int>();//存储vertex级的
        MultiNode *node = prevCoarsenGraph->Nodes[candidate];
        visited.insert(node->nodes.begin(), node->nodes.end());
        // candidate 的所有未match的邻居加入match
        for (unsigned int i:node->edges)
            visited.insert(prevCoarsenGraph->Nodes[i]->nodes.begin(), prevCoarsenGraph->Nodes[i]->nodes.end());
        // 对candidate的每个邻居检查其邻接点
        for (unsigned int vertexItera:visited) {
            // 检查这个邻居itera的邻接点数量
            Node *n = graph->graph_nodes.find(vertexItera)->second;
            for (Edge *iteraNei:n->edges) {
                // 如果该邻接点没有被match也没有在visited里，那么边界+1
                if (visited.count(iteraNei->node_id) == 0) { //currCoarsenGraph->Map_origin[iteraNei->node_id] == -1 &&
                    currborder++;
                }
            }
        }
        // 更新minimumBorder
        if (currborder < minBorderNum) {
            minBorderNum = currborder;
            minBorderNodeID = candidate;
        }
    }// end for

    // 将这个pivotNode的所有未match的node加入match集合（res）
    set<unsigned int> resSet;
    resSet.insert(minBorderNodeID);
    vector<unsigned int> mulNodeEdges = prevCoarsenGraph->Nodes[minBorderNodeID]->edges;
    for (unsigned int adjMnode:mulNodeEdges) {
        if (currCoarsenGraph->Map[adjMnode] == -1) resSet.insert(adjMnode);
    }
    for (unsigned int i:resSet) res.push_back(i);
}

void randomMatch(MultiNode *pivotNode, CoarsenGraph *currCoarsenGraph, vector<unsigned int> &res) {
    vector<unsigned int> unmatchedMNodeID = vector<unsigned int>();
    for (unsigned int n:pivotNode->edges) {
        if (currCoarsenGraph->Map[n] == -1) {
            unmatchedMNodeID.push_back(n);
        }
    }
    default_random_engine e;
    int resID;
    res.push_back(pivotNode->mnode_id);
    if (unmatchedMNodeID.size() != 0) {
        resID = unmatchedMNodeID[e() % unmatchedMNodeID.size()];
        res.push_back(resID);
    }
}

/**
 *
 * @param partitionID
 * @param matchedMultiNodePrevStage
 * @param currCoarsenGraph
 * @return
 */
MultiNode *matching(unsigned int partitionID, vector<MultiNode *> &matchedMultiNodePrevStage,
                    CoarsenGraph *currCoarsenGraph) {
    // 构建新MultiNode
    MultiNode *resMultiNode = new MultiNode();
    // 设置新MultiNode的mnode_id
    resMultiNode->mnode_id = partitionID;

    // 设置新MultiNode的成员数据
    for (MultiNode *mnodePtr: matchedMultiNodePrevStage) {
        // 更新Map
        currCoarsenGraph->Map[mnodePtr->mnode_id] = partitionID;

        // 设置新MultiNode的nodes
        resMultiNode->nodes.insert(resMultiNode->nodes.begin(), mnodePtr->nodes.begin(), mnodePtr->nodes.end());
        // 设置新MultiNodes的category
        set_union(resMultiNode->category.begin(), resMultiNode->category.end(), mnodePtr->category.begin(),
                  mnodePtr->category.end(), inserter(resMultiNode->category, resMultiNode->category.begin()));
        // 设置新MultiNode的prevMultiNode
        resMultiNode->prevMultiNodes.push_back(mnodePtr);
    }
    // 临时：检查结果中vertex是否有重复
//    set<unsigned int> s;
//    for(unsigned int node_id:resMultiNode->nodes){
//        if(s.count(node_id)>0){
//            cout<<"partition "<<resMultiNode->mnode_id<<"  vertex "<< node_id << " duplicates."<<endl;
//        }
//        s.insert(node_id);
//    }
    // 新MultiNode的edges先留空，等这个阶段所有的match完成再进行

    // 将match的结果记录在currCoarsenGraph的Map_origin成员中
    for (unsigned int node_id:resMultiNode->nodes) {
        currCoarsenGraph->Map_origin[node_id] = partitionID;
    }
    return resMultiNode;
}

/**
 * 对图进行粗化（Coarsening）并得到多个阶段的粗化结果
 * @param graph
 * @param coarsening_stage
 */
void coarsening(Graph *graph, vector<CoarsenGraph *> &coarsening_stage) {
    // 使用Graph构造第一代CoarsenGraph
    CoarsenGraph *initialCoarse = new CoarsenGraph(graph->graph_size);
    vector<int> Map_origin_initial = vector<int>(graph->graph_size);
    for (int i = 0; i < graph->graph_size; i++) {
        Map_origin_initial[i] = i;
    }
    initialCoarse->Map_origin = Map_origin_initial;
    // 构建initialCoarse的MultiNode对象
    for (int i = 0; i < graph->graph_size; i++) {
        // get node id
        unsigned int mnode_id = i;
        vector<unsigned int> nodes = {mnode_id};
        // get node edges
        vector<unsigned int> edges = vector<unsigned int>();
        Node *nptr = graph->graph_nodes.find(mnode_id)->second;
        for (Edge *edgePtr:nptr->edges) {
            edges.push_back(edgePtr->node_id);
        }
        // get node category set
        set<int> category = set<int>();
        category.insert(nptr->category);
        // build MultiNode object
        MultiNode *m = new MultiNode(mnode_id, nodes, edges, category);
        // insert into initialCoarse object
        initialCoarse->Nodes.push_back(m);
    }
    coarsening_stage.push_back(initialCoarse);
    // 主要循环 进行Coarsening的过程
    default_random_engine e;
    CoarsenGraph *currCoarseGraph, *prevCoarseGraph;
    prevCoarseGraph = initialCoarse;
    int coarseGraphSize = prevCoarseGraph->Nodes.size();
    while (coarseGraphSize > 2) { // 10
        cout << "size of currCoarseGraph: " << coarseGraphSize << endl;

        // 将上一个阶段的coarseGraph复制一份进行当前阶段的coarse
        // 初始化新的CoarseGraph，其Match和Map数组的大小是上一个阶段的CoarseGraph的收缩后partition的大小
        currCoarseGraph = new CoarsenGraph(prevCoarseGraph);// 使用上一个阶段的CoarseGraph进行初始化

        // 随机match
        // 构造一个随机序列，大小为coarse_graph_size，为后续match时的选择顺序
        // 构造序列，从0到coarse_graph_size有序
        vector<unsigned int> sequence = vector<unsigned int>(coarseGraphSize);
        for (int i = 0; i < coarseGraphSize; i++) {
            sequence[i] = i;
        }
        // 随机交换coarse_graph_size次,得到随机序列
        for (int i = 0; i < coarseGraphSize; i++) {
            unsigned int swapIndexA = e() % coarseGraphSize;
            unsigned int swapIndexB = e() % coarseGraphSize;
            // 交换
            int tmp;
            tmp = sequence[swapIndexA];
            sequence[swapIndexA] = sequence[swapIndexB];
            sequence[swapIndexB] = tmp;
        }

        // 随机match,按照sequence的序列顺序
        unsigned int partition_id = 0;
        for (int matchIteration = 0; matchIteration < coarseGraphSize; matchIteration++) {
            unsigned int matchPivotNodeID = sequence[matchIteration];
            if (currCoarseGraph->Map[matchPivotNodeID] != -1) { // 这个node之前已经被match过了
                continue;
            } else {
                // 进行match
                // 我将路网分成两种类型的子图：线性子图和稠密子图，对这两种类型的子图采取不同的match策略
                // 1. 线性子图：指的是子图中除了端点以外所有的结点的度都是2的子图。match策略：使用队列的方式不断寻找这条道路的全部结点，然后将这些结点match为一个MultiNode
                // 2. 稠密子图：指的是子图中有一个点的度比其他点高的子图。match策略：对当前点的所有相邻结点计算其未match的度，寻找度最高的一个结点作为match的中心，将它的所有相邻未match的结点match为一个MultiNode

                vector<unsigned int> toBeMatchNodeIDs = vector<unsigned int>();
                vector<MultiNode *> MultiNodeTobeMatch = vector<MultiNode *>();
                //MultiNodeTobeMatch.push_back(prevCoarseGraph->Nodes[matchPivotNodeID]); // 加入自己，确保至少能跟自己match

                MultiNode *pivotNode = prevCoarseGraph->Nodes[matchPivotNodeID];
                // 判断matchPivotNodeID的度（当前的coarsen图的度而不是原来路网的度）以决定使用哪种match策略
                if (prevCoarseGraph->Nodes[matchPivotNodeID]->edges.size() == 2) {// 度为2，使用线性子图match策略
                    linearMatch(pivotNode, prevCoarseGraph, currCoarseGraph, toBeMatchNodeIDs);
                } else {// 度不为2，使用稠密子图match策略
                    //denseMatch(pivotNode,prevCoarseGraph,currCoarseGraph,toBeMatchNodeIDs);
                    minimunBorderMatch(pivotNode, graph, prevCoarseGraph, currCoarseGraph, toBeMatchNodeIDs);
                    //randomMatch(pivotNode,currCoarseGraph,toBeMatchNodeIDs);
                }


                for (unsigned int n:toBeMatchNodeIDs) {
                    MultiNodeTobeMatch.push_back(prevCoarseGraph->Nodes[n]);
                }
                MultiNode *matchRes = matching(partition_id, MultiNodeTobeMatch, currCoarseGraph);
                currCoarseGraph->Nodes.push_back(matchRes);
                if (currCoarseGraph->Nodes[partition_id]->mnode_id != partition_id)
                    cout << " partition id dismatch!" << endl;
                partition_id += 1;
            }// end if coarse_graph->Match[toBeMatchIndex]!=-1
        }//end for   the whole matching

        // 设置currCoarseGraph的每个MultiNode中的Edges
        for (MultiNode *multinodePtr:currCoarseGraph->Nodes) {
            if (multinodePtr == nullptr) cout << "multinode is NULL!!" << endl;
            vector<unsigned int> multinode_nodes = multinodePtr->nodes;
            set<unsigned int> edgeNodesSet = set<unsigned int>();
            for (unsigned int n:multinode_nodes) {
                // 对于每一个node，在graph里面找到它的相邻结点在currCoarseGraph中所属的分区ID（使用Map_origin记录的信息），也就是相邻的MultiNode ID(它自己的除外)
                for (Edge *edgePtr:graph->graph_nodes.find(n)->second->edges) {
                    unsigned int adjNodeGraph = edgePtr->node_id;
                    unsigned int partitionID = currCoarseGraph->Map_origin[adjNodeGraph];
                    if (partitionID > 40000000) {
                        cout << "partitionID mapping error!" << endl;
                        //partitionID=0;
                    }
                    if (partitionID != multinodePtr->mnode_id)
                        edgeNodesSet.insert(partitionID);
                }// end for in edgeGraph
            }// end for in multinode_nodes
            // 将edgeSet中的邻接分区ID存入multiNode的edges中
            for (auto adjNodeID:edgeNodesSet) {
                multinodePtr->edges.push_back(adjNodeID);
            }
        }// end for in currCoarseGraph->Nodes
        coarsening_stage.push_back(currCoarseGraph);
        coarseGraphSize = currCoarseGraph->Nodes.size();
        prevCoarseGraph = currCoarseGraph;
    }// end while

}// end function
