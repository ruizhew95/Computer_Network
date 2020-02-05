#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <iostream>
#include <string>
using std::cout; 
using std::endl;
using std::string;  

//Command Line Arg Handler
//returns struct cmdArgs holding parsed input

struct cmdArgs {
	int dns;   
	int listenPort; 
	std::string wwwIp;  
	double alpha;  
	std::string log; 
	int dnsIp; 
	int dnsPort; 
	} ; 

struct cmdArgs parser(int argCount, char* args[]){
	//Check for num args. 6 for noDNS, 7 for DNS
	if (argCount != 6){
		if (argCount != 7){
		cout << "Wrong # of arguments" << endl; 
		exit(1); 
		}
	}
	
	cmdArgs values; 
	string nodns = "--nodns"; 
	
	//if nodns flag, return dns as 0 
	if (args[1] == nodns){
		values.dns = 0;
		values.listenPort = atoi(args[2]); 
		values.wwwIp = args[3]; 
		values.alpha = atof(args[4]); 
		values.log = args[5]; 
	} 
	//if dns flag, return dns as 1
	else{
		values.dns = 1;
		values.listenPort = atoi(args[2]); 
		values.dnsIp = atoi(args[3]); 
		values.dnsPort = atoi(args[4]); 
		values.alpha = atof(args[5]); 
		values.log = args[6];  
	}
	
	return values; 
}

#endif
