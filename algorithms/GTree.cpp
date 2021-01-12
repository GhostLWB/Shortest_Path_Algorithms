#include "GTree.h"



GNode::GNode() {
    this->border = vector<unsigned int>();
    this->categories = set<int>();
    this->distMatrix = map<string, double>();
    this->childGNode = vector<GNode *>();
}

GTree::GTree(vector<CoarsenGraph *> &stages, Graph *graph) {
    A_Star aStar(graph);
	this->computedDistance=map<string,double>();
	loadDistanceFromFile();

    this->leaf = vector<GNode *>(graph->graph_size, nullptr);

    // 构造根节点
    this->root = new GNode();
    // 对最后一个stage的每个multiNode进行计算，构造根节点剩下的成员
    int stageSize=stages.size();
    CoarsenGraph *coarsenGraph = stages[stageSize - 1];


    // 构造根节点的边界：根节点没有边界
    this->root->border=vector<unsigned int>();
    // 构造根节点的categories，子节点
    for(MultiNode *multiNode: coarsenGraph->Nodes){
        GNode* childNode=buildGNode(multiNode,stages,stageSize-1,graph,this->leaf);
        this->root->childGNode.push_back(childNode);
        set_union(this->root->categories.begin(), this->root->categories.end(), multiNode->category.begin(),
                  multiNode->category.end(), inserter(this->root->categories, this->root->categories.begin()));
    }

    // 计算根节点的distMatirx
    // 获取所有子图的边界
    vector<unsigned int> allBorders = vector<unsigned int>();
    for (GNode *childNode:this->root->childGNode) {
        allBorders.insert(allBorders.begin(), childNode->border.begin(), childNode->border.end());
    }
	this->root->border=allBorders;
	//cout<<"root node border size: "<<allBorders.size()<<endl;

    // 计算任意两个边界结点之间的距离
    sort(allBorders.begin(), allBorders.end());
    int borderSize = allBorders.size();
    for (int i = 0; i < borderSize - 1; i++) {
        for (int j = i + 1; j < borderSize; j++) {
            double distance;
            string distKey = to_string(allBorders[i]) + "-" + to_string(allBorders[j]);
            if(this->computedDistance.find(distKey)==this->computedDistance.end()){
                distance = aStar.ShortestDistance(allBorders[i], allBorders[j]);
            }else{
                distance=this->computedDistance.find(distKey)->second;
            }
            this->root->distMatrix[distKey] = distance;
        }
    }// end for traversing allBorders
	saveDistanceToFile(this->root);
} // end function

GNode *GTree::buildGNode(MultiNode *MNode, vector<CoarsenGraph *> &stages, int coarseGraphIndex, Graph *graph,
                         vector<GNode *> &leaf) {
    A_Star aStar(graph);
    cout<<"building GNode using Multinode of stage: "<< coarseGraphIndex<<endl;
    // 获取coarseGraph指针
    CoarsenGraph *coarsenGraph = stages[coarseGraphIndex];
    // 初始化GNode类成员
    GNode *resultNode = new GNode();
	set<unsigned int> borderSet=set<unsigned int>();
    vector<unsigned int> borders = vector<unsigned int>();
    map<string, double> distMatrix = map<string, double>();
    vector<GNode *> childGNode = vector<GNode *>();
    resultNode->categories = MNode->category;

    // 寻找MNode中vertexes的边界
    for (unsigned int vertexID:MNode->nodes) {
        // 如果vertex的所有邻接结点中有结点的分区不是MNode的分区，那么这个vertex就是一个边界
        // 寻找vertex的邻接结点(从graph中找)

        Node *vertex = graph->graph_nodes.find(vertexID)->second;
        if(vertex==nullptr){
            cout<<" error : Node* is null, vertex id: "<<vertexID<<endl;
            break;
        }
        vector<Edge*> vertexEdges = vertex->edges;
        for (Edge *edgeIterator:vertexEdges) {
            unsigned int neighborNodeID = edgeIterator->node_id;
            // 判断该邻接结点属于当前stage的哪个partition（使用Map_origin）
            unsigned int partitionBelongto = coarsenGraph->Map_origin[neighborNodeID];
            if (partitionBelongto != MNode->mnode_id) {
                // 那么这个vertexID所代表的vertex就是一个border
                borderSet.insert(vertexID);
            }// end if
        } // end for traversing vertexEdges
    }// end for traversing MNode->nodes for finding borders

	for(unsigned int setIterator: borderSet){
		borders.push_back(setIterator);
	}
    resultNode->border = borders;

	cout<<"total vertex size: "<<MNode->nodes.size()<<"  border size: "<<resultNode->border.size()<<endl;

    // 判断当前节点是不是叶节点
    if (coarseGraphIndex <= stages.size()/2 ) { // 当前partition的结点数量小于某个值，是一个叶节点   MNode->nodes.size() < 100

        // 计算所有边界到子图内部结点的最短距离
        for (unsigned int borderVertex:borders) {
            //cout<<"border vertex: "<<borderVertex<<endl;
            for (unsigned int insideVertex:MNode->nodes) {
                //cout<<borderVertex<<" to "<<insideVertex;
                double distance;
                string distKey =
                        to_string(min(borderVertex, insideVertex)) + "-" + to_string(max(borderVertex, insideVertex));                if(this->computedDistance.find(distKey)==this->computedDistance.end()){
                    distance = aStar.ShortestDistance(borderVertex, insideVertex);
                }else{
                    distance=this->computedDistance.find(distKey)->second;
                }
                //cout<<"  distance:"<< distance<<endl;
                // 将边界到子图内部结点的最短距离保存在distMatrix里
                distMatrix[distKey] = distance;
            }
        }// end for :traversing borders

        resultNode->distMatrix = distMatrix;
        resultNode->childGNode.push_back(nullptr);

        // 记录leaf
        for (unsigned int graphVertex:MNode->nodes) {
            leaf[graphVertex] = resultNode;
        }

    } else {// 当前partition的结点数量大于等于70，是非叶节点
        // 递归计算GTree的子节点
        for (MultiNode *prevMultiNodeIterator:MNode->prevMultiNodes) {
            GNode *childNode = buildGNode(prevMultiNodeIterator, stages, coarseGraphIndex - 1, graph, leaf);
            resultNode->childGNode.push_back(childNode);
        }
        // 计算所有子图的边界之间的最短距离
        // 获取所有子图的边界
        vector<unsigned int> allBorders = vector<unsigned int>();
        for (GNode *childNode:resultNode->childGNode) {
            allBorders.insert(allBorders.begin(), childNode->border.begin(), childNode->border.end());
        }

        // 计算任意两个边界结点之间的距离
        sort(allBorders.begin(), allBorders.end());
        int borderSize = allBorders.size();
        for (int i = 0; i <= borderSize - 1; i++) {
            for (int j = i ; j < borderSize; j++) {
                double distance;
                string distKey = to_string(allBorders[i]) + "-" + to_string(allBorders[j]);
                if(this->computedDistance.find(distKey)==this->computedDistance.end()){
                    distance = aStar.ShortestDistance(allBorders[i], allBorders[j]);
                }else{
                    distance=this->computedDistance.find(distKey)->second;
                }
                distMatrix[distKey] = distance;
            }
        }// end for traversing allBorders

        resultNode->distMatrix = distMatrix;
    }// end if : whether current node is a leaf :else section
	saveDistanceToFile(resultNode);
    return resultNode;
}// end function

GNode* GTree::findCommonAncestor(GNode *root,GNode *nodeA,GNode *nodeB){
    if(root==nullptr or root==nodeA or root==nodeB) return root;
    int foundCount=0;
    GNode *found=nullptr;
    for(GNode* child:root->childGNode){
        GNode *res=findCommonAncestor(child,nodeA,nodeB);
        if(res!=nullptr){
            foundCount++;
            found=res;
        }
    }
    if(foundCount==0) return nullptr;
    if(foundCount==1) return found;
    return root;
}
bool GTree::containsGNode(GNode *root,GNode *targetGNode){
    if(root==nullptr) return false;
    if(root==targetGNode) return true;
    bool res=false;
    for(GNode *child:root->childGNode){
        res = res or containsGNode(child,targetGNode);
    }
    return res;
}
double GTree::shortestDistance(unsigned int nodeA,unsigned int nodeB,Graph* graph){
    // 首先获取两个查询结点所在的叶节点
    GNode *gnodeA=this->leaf[nodeA],*gnodeB=this->leaf[nodeB];
    // 判断两个查询结点是否在同一个叶节点上
    if(gnodeA==gnodeB){
        //cout<<"the same leaf"<<endl;
        string queryKey=to_string(min(nodeA,nodeB))+"-"+to_string(max(nodeA,nodeB));
        if(gnodeA->distMatrix.find(queryKey)==gnodeA->distMatrix.end()){
            A_Star aStar(graph);
            return aStar.ShortestDistance(nodeA,nodeB);
        }
        return gnodeA->distMatrix.find(queryKey)->second;
    }else{ // 不在同一个叶节点上，就用assembley方法
        // 首先寻找两个叶节点的最近公共祖先节点
        GNode *commonAncestor=findCommonAncestor(this->root,gnodeA,gnodeB);
        // 找到从A到祖先的路径
        vector<GNode*> pathToA=vector<GNode*>();
        GNode *currGNode=commonAncestor;
        while(currGNode!=nullptr){
            for(GNode* child:currGNode->childGNode){
                if(child==nullptr){
                    currGNode=child;
                    break;
                }
                if(containsGNode(child,gnodeA)){
                    pathToA.push_back(child);
                    currGNode=child;
                    break;
                }
            }// end for
        }// end while
        reverse(pathToA.begin(),pathToA.end());// A ......(A在第一个位置)

        // 找到从祖先到B的路径
        vector<GNode*> pathToB=vector<GNode*>();
        currGNode=commonAncestor;
        while(currGNode!=nullptr){
            for(GNode* child:currGNode->childGNode){
                if(child==nullptr){
                    currGNode=child;
                    break;
                }
                if(containsGNode(child,gnodeB)){
                    pathToB.push_back(child);
                    currGNode=child;
                    break;
                }
            }// end for
        }// end while
        // ........B (B在最后一个位置)

        vector<GNode*> gnodePath;
        gnodePath.insert(gnodePath.begin(),pathToA.begin(),pathToA.end());
        gnodePath.insert(gnodePath.end(),pathToB.begin(),pathToB.end());
        // A........ ..........B

        // 使用动态规划的方式计算最短路径
        int pathLength=gnodePath.size();
        vector<vector<double>> GDP=vector<vector<double>>(pathLength);
        for(int NI=0;NI<pathLength;NI++){
            int gnodeBorderSize=gnodePath[NI]->border.size();
            GDP[NI]=vector<double>(gnodeBorderSize,DBL_MAX);
            if(NI==0){
                for(unsigned int i=0;i<gnodeBorderSize;i++){
                    unsigned int borderNode=gnodePath[NI]->border[i];
                    string key=to_string(min(nodeA,borderNode))+"-"+to_string(max(nodeA,borderNode));
                    GDP[NI][i]=gnodePath[NI]->distMatrix.find(key)->second;
                }
            }else{
                for(unsigned int i=0;i<gnodeBorderSize;i++){
					unsigned int borderNode=gnodePath[NI]->border[i];
					double minDist=DBL_MAX;
                    int preGNodeBorderSize=gnodePath[NI-1]->border.size();
                    for(unsigned int j=0;j<preGNodeBorderSize;j++){
						unsigned int preBorderNode=gnodePath[NI-1]->border[j];
						string key = to_string(min(preBorderNode,borderNode))+"-"+to_string(max(preBorderNode,borderNode));
						double dist=-1;
						if(NI==pathToA.size()){// 在计算从LCA到B的第一个节点（也就是第pathToA.size()个节点的时候，距离要从LCA的距离矩阵中读取）
							dist=commonAncestor->distMatrix.find(key)->second;
						}else if(NI<pathToA.size()){
                            dist=gnodePath[NI]->distMatrix.find(key)->second;
						}else if(NI>pathToA.size()){
                            dist=gnodePath[NI-1]->distMatrix.find(key)->second;
						}
						if(dist==-1){
						    cout<<"error: distance "<<key<<" not found"<<endl;
						}
						minDist=min(minDist,GDP[NI-1][j]+dist);
                    }
                    GDP[NI][i]=minDist;
                }
            }// end if NI == 0 else section
        }// end for iterating NI

		double resultDistance=DBL_MAX;
		for(int j=0;j<gnodePath[pathLength-1]->border.size();j++){
			unsigned int borderNode=gnodePath[pathLength-1]->border[j];
			string key=to_string(min(nodeB,borderNode))+"-"+to_string(max(nodeB,borderNode));
			resultDistance=min(resultDistance,GDP[pathLength-1][j]+gnodePath[pathLength-1]->distMatrix.find(key)->second);
		}
		return resultDistance;
    }// end if query node is in the same leaf
}// end function

/**
 *
 * @param gnode
 */
void GTree::saveDistanceToFile(GNode *gnode){
	string distance_file_path=DISTANCE_FILE;
	ofstream distance_file(distance_file_path.c_str(),ios::app);
	if(!distance_file.is_open()){
		cout<<"error opening file:"<<distance_file_path<<" !"<<endl;
		return;
	}
	for(map<string,double>::iterator iter=gnode->distMatrix.begin();iter!=gnode->distMatrix.end();iter++){
		if(this->computedDistance.find(iter->first)==this->computedDistance.end()){
            distance_file<<iter->first<<" "<<iter->second<<endl;
		}
	}
	distance_file.close();
}

/**
 *
 */
void GTree::loadDistanceFromFile(){
	string distance_file_path=DISTANCE_FILE;
	ifstream distance_file(distance_file_path.c_str());
	if(!distance_file.is_open()){
		cout<<"error opening file: "<<distance_file_path<<" !"<<endl;
		return;
	}
	cout<<"loading distance file..."<<endl;
	string buffer_string;
	while(getline(distance_file,buffer_string)){
		stringstream ss;
		string key;
		double distance;
		unsigned int start_pos,end_pos;

		start_pos=0;
		end_pos=buffer_string.find(' ',start_pos);
		key=buffer_string.substr(start_pos,end_pos-start_pos);
		
		start_pos=end_pos+1;
		end_pos=buffer_string.find(' ',start_pos);
		string distance_string=buffer_string.substr(start_pos,end_pos-start_pos);
		ss<<distance_string;
		ss>>distance;

		this->computedDistance[key]=distance;
	}// end while
	distance_file.close();
}// end function
