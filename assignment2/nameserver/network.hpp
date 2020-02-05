#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>

class Network{
  enum NodeEnum {
    CLIENT = 1,
    SERVER = 2,
    SWITCH = 3,
  };

  class Node{
    public:
        NodeEnum type;
        std::string ipaddr;
  };

  // List of nodes
  std::vector <Node> Nodes;
  // Adjacency matrix of links
  std::vector<std::vector< int > > map;
    public: 
        void init();
        void printGraph();
};

void Network::init(){
    std::ifstream geo;
    geo.open("geo_dist.txt");
    if(!geo.is_open()){
        std::cout<<"Error on opening"<<std::endl;
        exit(1);
    }
    
    Node node;
    std::string token, type, ip_addr;
    int num_nodes, node_id;
    int num_links;

    geo>>token>>num_nodes; 
    for(int i=0; i<num_nodes; i++){
        geo>>node_id>>type>>ip_addr;
        node.ipaddr = ip_addr;
        if(type == "CLIENT") node.type = CLIENT;
        else if(type == "SWITCH") node.type = SWITCH;
        else if(type == "SERVER") node.type = SERVER;
        else {
            std::cout<<"no matching type"<<std::endl;
            exit(1);
        }
        Nodes.push_back(node);
    }

    //initialize adjacency matrix with INT32_MAX
    for(int i=0; i<num_nodes; i++){
        std::vector<int> init;
        for(int j=0; j<num_nodes; j++)
            init.push_back(INT32_MAX);
        map.push_back(init);
    }

    geo>>token>>num_links;
    int id1, id2, cost;
    for(int i=0; i<num_links; i++){
        geo>>id1>>id2>>cost;
        map[id1][id2] = cost;
        map[id2][id1] = cost;
    }

}

void Network::printGraph(){
    for(int i=0; i<(int)Nodes.size();i++){
        std::cout<<i<<" "<<Nodes[i].type<<" "<<Nodes[i].ipaddr<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Nodes:"<<"    "<<0<<"     "<<1<<"     "<<"       "<<2<<"      "<<3<<"     "<<4<<"        "<<5<<std::endl;
    for(int i=0; i<(int)map.size(); i++){
        std::cout<<i<<" ";
        for(int j=0; j<(int)map[i].size();j++)
            std::cout<<map[i][j]<<" ";
        std::cout<<std::endl;
    } 
}


