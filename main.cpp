#include "algorithms/graph.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/A_Star.h"
#include "algorithms/GraphPartition.h"
#include "algorithms/GTree.h"
#include <random>
/**
 * display the query information and query result
 * @param testcaseID
 * @param nodeS
 * @param nodeT
 * @param distance
 * @param pathResult
 */
void displayResult(string algorithmName,unsigned int testcaseID,unsigned int nodeS,unsigned int nodeT,double distance,const vector<unsigned int> & pathResult,double time){
    cout<<"algorithm: "<<algorithmName<<" "<<endl;
    cout<<"testcase: "<<testcaseID<<". start node:"<<nodeS<<". end node: "<<nodeT<<endl;
    cout<<"the shortest distance is:"<<distance<<endl;
    cout<<"the path is :";
    int pathsize=pathResult.size();
    for(int i=0;i<pathsize;i++){
        if(i!=pathsize-1){
            cout<<pathResult[i]<<"->";
        }else{
            cout<<pathResult[i]<<endl;
        }
    }
    cout<<"time used: "<<time<<" seconds."<<endl<<endl;
}

int main() {
    // load road file  (the road dataset is specified in file /algorithms/configure.h)
    Graph* graph=new Graph();
    cout<<"the number of nodes in graph is: "<<graph->graph_size<<endl;

    Dijkstra dijkstra(graph);
    A_Star aStar(graph);

    vector<CoarsenGraph*> coarsenStage=vector<CoarsenGraph*>();
    coarsening(graph,coarsenStage);
    GTree gTree(coarsenStage,graph);

    vector<vector<double>> timeComsumings=vector<vector<double>>(4);
    srand((unsigned )time(NULL));
    int testcaseNum=100;
    for(int i=0;i<testcaseNum;i++){

        unsigned int nodeS=rand()%graph->graph_size;
        unsigned int nodeT=rand()%graph->graph_size;


        // ======================= Dijkstra with priority queue =======================================================
        clock_t start = 0, finish = 0;
        start = clock();
        vector<unsigned int> pathResult;
        pathResult=vector<unsigned int> ();
        //double distance= dijkstra.ShortestPathPriorityQueue(nodeS, nodeT, pathResult);
        double distance=dijkstra.ShortestDistancePriorityQueue(nodeS,nodeT);
        finish=clock();
        double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        timeComsumings[0].push_back(totaltime);
        displayResult("Dijkstra priority queue",i,nodeS,nodeT,distance,pathResult,totaltime);
        // ============================================================================================================


        // ======================= Dijkstra classic ===================================================================
        start = 0, finish = 0;
        start = clock();
        pathResult=vector<unsigned int> ();
        //distance= dijkstra.ShortestPath(nodeS, nodeT, pathResult);
        distance=dijkstra.ShortestDistance(nodeS,nodeT);
        finish=clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        timeComsumings[1].push_back(totaltime);
        displayResult("Dijkstra classic",i,nodeS,nodeT,distance,pathResult,totaltime);
         //===========================================================================================================


         // ======================= A * shortest with plane distance as heuristic ====================================
        start = 0, finish = 0;
        start = clock();
        pathResult=vector<unsigned int> ();
        //distance= aStar.ShortestPath(nodeS, nodeT, pathResult);
        distance=aStar.ShortestDistance(nodeS,nodeT);
        finish=clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        timeComsumings[2].push_back(totaltime);
        displayResult("A Star",i,nodeS,nodeT,distance,pathResult,totaltime);
        //============================================================================================================

        // ======================= G Tree shortest distance algorithm ===============================================
        start = 0, finish = 0;
        start = clock();
        pathResult=vector<unsigned int> ();
        // GTree has no shortest path version
        distance=gTree.shortestDistance(nodeS,nodeT,graph);
        finish=clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        timeComsumings[3].push_back(totaltime);
        displayResult("G Tree",i,nodeS,nodeT,distance,pathResult,totaltime);
        //============================================================================================================
    }

    for(int i=0;i<4;i++){
        cout<<"algorithm "<<i<<endl;
        for(int j=0;j<testcaseNum;j++){
            cout<<timeComsumings[i][j]<<",";
        }
        cout<<endl;
    }
    return 0;
}
