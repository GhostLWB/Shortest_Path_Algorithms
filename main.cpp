#include "algorithms/graph.h"
#include "algorithms/Dijkstra.h"
#include "algorithms/A_Star.h"
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
    // global variable
    unordered_map<unsigned int, set<Road> > graph; // the road network
    unordered_map<unsigned int, pair<double,double>> lonlat; // node_id longitude latitude
    read_file("../dataset/map_california.txt",graph);
    read_lonlat("../dataset/lonlat_california.txt",lonlat);
    unsigned int graphsize=graph.size();
    cout<<"the number of nodes in graph is: "<<graphsize<<endl;

    Dijkstra dijkstra(graph);
    A_Star aStar(graph,lonlat);

    srand((unsigned )time(NULL));
    for(int i=0;i<100;i++){

        unsigned int nodeS=rand()%graphsize;
        unsigned int nodeT=rand()%graphsize;
        //unsigned int nodeS=15;
        //unsigned int nodeT=7;

        clock_t start = 0, finish = 0;
        start = clock();
        vector<unsigned int> pathResult;
        pathResult=vector<unsigned int> ();
        //double distance= dijkstra.ShortestPathPriorityQueue(nodeS, nodeT, pathResult);
        double distance=dijkstra.ShortestDistancePriorityQueue(nodeS,nodeT);
        finish=clock();
        double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        displayResult("Dijkstra priority queue",i,nodeS,nodeT,distance,pathResult,totaltime);

        /*
        start = 0, finish = 0;
        start = clock();
        pathResult=vector<unsigned int> ();
        distance= dijkstra.ShortestPath(nodeS, nodeT, pathResult);
        //distance=dijkstra.ShortestDistance(nodeS,nodeT);
        finish=clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        displayResult("Dijkstra priority classic",i,nodeS,nodeT,distance,pathResult,totaltime);
         */

        start = 0, finish = 0;
        start = clock();
        pathResult=vector<unsigned int> ();
        //distance= aStar.ShortestPath(nodeS, nodeT, pathResult);
        distance=aStar.ShortestDistance(nodeS,nodeT);
        finish=clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        displayResult("A Star",i,nodeS,nodeT,distance,pathResult,totaltime);

    }

    return 0;
}
