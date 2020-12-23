//
// Created by Dell on 2020/12/16.
//

#include "graph.h"
/**
* read road data from text file and build the graph
* @param file_path
* @param graph
* @return void  the road network data is stored in global variable "graph"
*/
void read_file(string file_path,unordered_map<unsigned int, vector<Edge> >& graph) {

    //read file
    clock_t start = 0, finish = 0;
    start = clock();
    ifstream input_file(file_path.c_str());
    if (!input_file.is_open()) {
        cout << "error opening file" << file_path << endl;
    }
    string buffer_string;
    while (getline(input_file,buffer_string)) {
        stringstream ss;
        // extract start node ID
        unsigned long start_pos = 0;
        unsigned long end_pos = buffer_string.find(' ', start_pos);
        string start_NID_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss << start_NID_string;
        unsigned int start_NID;
        ss >> start_NID;


        // extract end node ID
        start_pos = end_pos + 1;
        end_pos = buffer_string.find(' ', start_pos);
        string end_NID_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss.clear();
        ss << end_NID_string;
        unsigned int end_NID;
        ss >> end_NID;
        //cout<<"end_nid: "<<end_NID<<endl;


        // extract length
        start_pos = end_pos + 1;
        end_pos = buffer_string.find(' ', start_pos);
        string road_length_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss.clear();
        ss << road_length_string;
        double road_length;
        ss >> road_length;
        //cout<<"road_length: "<<road_length<<endl;


        //cout<<"start: "<<start_NID<<", end: "<<end_NID<<endl;
        Edge e_star=Edge(end_NID,road_length);
        Edge e_end=Edge(start_NID,road_length);

        // find road set according to start node id
        auto iterator1 = graph.find(start_NID);
        if (iterator1 != graph.end()) {
            vector<Edge> value = iterator1->second;
            value.push_back(e_star);
            graph[start_NID] = value;
        } else {
            vector<Edge> value;
            value.push_back(e_star);
            graph[start_NID] = value;
        }

        // find road set according to end node id
        iterator1 = graph.find(end_NID);
        if (iterator1 != graph.end()) {
            vector<Edge> value = iterator1->second;
            value.push_back(e_end);
            graph[end_NID] = value;
        } else {
            vector<Edge> value;
            value.push_back(e_end);
            graph[end_NID] = value;
        }
        //cout<<graph.size()<<endl;
    }
    input_file.close();
    finish = clock();
    double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "<load file> time consume:" << totaltime << " secondes" << endl;
}

/**
 *  read longitude and latitude data from file
 * @param file_path
 * @param lonlat
 * @return void
 */
void read_lonlat(string file_path,unordered_map<unsigned int, pair<double,double>>& lonlat){
    //read file
    clock_t start = 0, finish = 0;
    start = clock();
    ifstream input_file(file_path.c_str());
    if (!input_file.is_open()) {
        cout << "error opening file" << file_path << endl;
    }
    string buffer_string;
    while (getline(input_file,buffer_string)) {
        stringstream ss;
        // extract node ID
        unsigned long start_pos = 0;
        unsigned long end_pos = buffer_string.find(' ', start_pos);
        string NID_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss << NID_string;
        unsigned int NID;
        ss >> NID;


        // extract longitude
        start_pos = end_pos + 1;
        end_pos = buffer_string.find(' ', start_pos);
        string LON_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss.clear();
        ss << LON_string;
        double LON;
        ss >> LON;


        // extract latitude
        start_pos = end_pos + 1;
        end_pos = buffer_string.find(' ', start_pos);
        string LAT_string = buffer_string.substr(start_pos, end_pos - start_pos);
        ss.clear();
        ss << LAT_string;
        double LAT;
        ss >> LAT;


        // insert into map lonlat
        lonlat[NID]={LON,LAT};

    }
    input_file.close();
    finish = clock();
    double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "<load file> time consume:" << totaltime << " secondes" << endl;
}