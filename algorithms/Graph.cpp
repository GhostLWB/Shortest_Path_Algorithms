#include "Graph.h"

Node::Node(unsigned int input_node_id){
	node_id=input_node_id;
	this->category=-1;
	this->lonlat={0,0};
	this->edges=vector<Edge*>();
}

Node::Node(unsigned int input_node_id,int input_category){
	node_id=input_node_id;
	category=input_category;
	this->lonlat={0,0};
	this->edges=vector<Edge*>();
}
Edge::Edge(unsigned int input_node_id,double input_distance){
	node_id=input_node_id;
	distance=input_distance;
}
Graph::Graph(){

	// define class member variable
	this->graph_nodes=unordered_map<unsigned int, Node*> ();
	
	string edge_file_path=EDGE_FILE;
	// load edge data from file and build [graph_node->edges]
	ifstream edge_file(edge_file_path.c_str());
	if(!edge_file.is_open()){
		cout<<"error opening file: "<<edge_file_path<<" !"<<endl;
	}else{
        cout<<"Graph:: loading edge file: "<<edge_file_path<<"..."<<endl;
        string buffer_string;
        while(getline(edge_file,buffer_string)){
            // line format:
            // node_id node_id distance

            stringstream ss;
            unsigned int start_pos;
            unsigned int end_pos;
            unsigned int start_node_id,end_node_id;
            double road_length;

            // extract start node id
            start_pos=0;
            end_pos=buffer_string.find(' ',start_pos);
            string start_nid_string=buffer_string.substr(start_pos,end_pos-start_pos);
            ss<<start_nid_string;
            ss>>start_node_id;
            ss.clear();

            // extract end node id
            start_pos=end_pos+1;
            end_pos=buffer_string.find(' ',start_pos);
            string end_nid_string=buffer_string.substr(start_pos,end_pos-start_pos);
            ss<<end_nid_string;
            ss>>end_node_id;
            ss.clear();

            // extract length
            start_pos=end_pos+1;
            end_pos=buffer_string.find(' ',start_pos);
            string road_length_string=buffer_string.substr(start_pos,end_pos-start_pos);
            ss<<road_length_string;
            ss>>road_length;

            // build Edge
            Edge* edge_for_start=new Edge(end_node_id,road_length); // edge for start node
            Node* node_start;
            auto iterator_node_start=this->graph_nodes.find(start_node_id);
            if(iterator_node_start!=this->graph_nodes.end()){// graph already have recore of this node
                node_start=iterator_node_start->second;
                node_start->edges.push_back(edge_for_start);
            }else{// this node didnot appear in graph before, make a new one
                node_start = new Node(start_node_id);
                node_start->edges.push_back(edge_for_start);
            }
            this->graph_nodes[start_node_id]=node_start;


            Edge* edge_for_end=new Edge(start_node_id,road_length); // edge for end node
            Node* node_end;
            auto iterator_node_end=this->graph_nodes.find(end_node_id);
            if(iterator_node_end!=this->graph_nodes.end()){
                node_end=iterator_node_end->second;
                node_end->edges.push_back(edge_for_end);
            }else{
                node_end=new Node(end_node_id);
                node_end->edges.push_back(edge_for_end);
            }

            this->graph_nodes[end_node_id]=node_end;
        } // end of while
        edge_file.close();
    }

	// load  lonlat data from file and build [graph_nodes.lonlat]
	string lonlat_file_path=LONLAT_FILE;
	ifstream lonlat_file(lonlat_file_path.c_str());
	if(!lonlat_file.is_open()){
		cout<<"error opening file: "<<lonlat_file_path<<" !"<<endl;
	}else{
        cout<<"Graph:: loading lonlat file: "<<lonlat_file_path<<" ....."<<endl;
        string lonlat_buffer;
        while(getline(lonlat_file,lonlat_buffer)){
            stringstream ss;
            unsigned int node_id;
            double longitude,latitude;
            unsigned int start_pos,end_pos;

            // extract node id
            start_pos=0;
            end_pos=lonlat_buffer.find(' ',start_pos);
            string node_id_string=lonlat_buffer.substr(start_pos,end_pos-start_pos);
            ss<<node_id_string;
            ss>>node_id;
            ss.clear();

            // extract longitude
            start_pos=end_pos+1;
            end_pos=lonlat_buffer.find(' ',start_pos);
            string longitude_string=lonlat_buffer.substr(start_pos,end_pos-start_pos);
            ss<<longitude_string;
            ss>>longitude;
            ss.clear();

            // extract latitude
            start_pos=end_pos+1;
            end_pos=lonlat_buffer.find(' ',start_pos);
            string latitude_string=lonlat_buffer.substr(start_pos,end_pos-start_pos);
            ss<<latitude_string;
            ss>>latitude;
            ss.clear();

            // build lonlat entry
            this->graph_nodes[node_id]->lonlat={longitude,latitude};
        }
        lonlat_file.close();
	}


	// load node category data from file and build [graph_nodes->category]
	string node_category_file_path=CATEGORY_FILE;
	ifstream node_file(node_category_file_path.c_str());
	if(!node_file.is_open()){
		cout<<"error opening file: "<<node_category_file_path<<endl;
	}else{
        cout<<"Graph:: loading category file : "<<node_category_file_path<<" ..."<<endl;
        string node_category_buffer;
        // node file format should be : node_id category
        while(getline(node_file,node_category_buffer)){
            stringstream ss;
            unsigned int node_id;
            int category;
            unsigned int start_pos,end_pos;

            // extract node id
            start_pos=0;
            end_pos=node_category_buffer.find(' ',start_pos);
            string node_id_string=node_category_buffer.substr(start_pos,end_pos-start_pos);
            ss<<node_id_string;
            ss>>node_id;
            ss.clear();

            // extract category
            start_pos=end_pos+1;
            end_pos=node_category_buffer.find(' ',start_pos);
            string category_string=node_category_buffer.substr(start_pos,end_pos-start_pos);
            ss<<category_string;
            ss>>category;
            ss.clear();

            // build Node category
            this->graph_nodes[node_id]->category=category;
        }
        this->graph_size=this->graph_nodes.size();
        node_file.close();
        for(int i=0;i<this->graph_size;i++){
            if(this->graph_nodes.find(i)==this->graph_nodes.end()){
                Node* n=new Node(i,-1);
                this->graph_nodes[i]=n;
            }else{
                continue;
            }
        }
	}

}

