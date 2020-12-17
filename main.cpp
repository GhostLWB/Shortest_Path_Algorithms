#include "graph.h"
#include <random>
#include "algorithms/Dijkstra.h"
/**
 * display the query information and query result
 * @param testcaseID
 * @param nodeS
 * @param nodeT
 * @param distance
 * @param pathResult
 */
void displayResult(unsigned int testcaseID,unsigned int nodeS,unsigned int nodeT,double distance,const vector<unsigned int> & pathResult,double time){
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
    cout<<"time used: "<<time<<" seconds."<<endl;
}

int main() {
    read_file("../dataset/map_california.txt");
    unsigned int graphsize=graph.size();
    cout<<"the number of nodes in graph is: "<<graphsize<<endl;

    Dijkstra dijkstra(graph);

    for(int i=0;i<100;i++){
        unsigned int nodeS=rand()%graphsize;
        unsigned int nodeT=rand()%graphsize;

        clock_t start = 0, finish = 0;
        start = clock();
        vector<unsigned int> pathResult;
        double distance=dijkstra.compute_shortest_path(nodeS,nodeT,pathResult);
        finish=clock();
        double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
        displayResult(i,nodeS,nodeT,distance,pathResult,totaltime);
    }

    return 0;
}
