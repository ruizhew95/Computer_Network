#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>


using std::string; 


struct Robin {
	
	std::vector<string> addrs; 
	int state; 
	int numAddrs; 
}; 

//Given Robin struct, returns the next IP to be used as a STRING
string updateRobin(struct Robin& robin){
	
	string toReturn = robin.addrs[robin.state];
	
	robin.state++; 
	
	if (robin.state == robin.numAddrs){
		robin.state = 0; 
	}
	
	return toReturn; 			
}

//Given reference to ifstream, returns Robin struct containing state and IP's. 
struct Robin makeRobin(std::ifstream& file){
	
	struct Robin robin; 
	robin.numAddrs = 0; 
	robin.state = 0; 

	std::string input = ""; 

	while(std::getline(file, input)){
		robin.addrs.push_back(input); 
		robin.numAddrs++; 
	}
	
	return robin; 
}	









#endif
