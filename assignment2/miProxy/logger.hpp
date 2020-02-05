#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

void logger(std::string log, std::string browserIp, std::string chunkName, std::string serverIp, double duration, double tput, double avgTput, double bitRate) {
	
	std::ofstream file; 
	//Open file in append mode. If doesn't exist, will create file.
	//Else, appends to end of log. 
	file.open(&log[0], std::fstream::app); 

	if (!file.is_open()){
		std::cout << "ERROR: FILE NOT OPENED>>> " << log << std::endl; 
		exit(0); 
	}

	file << browserIp << " " << chunkName << " " << serverIp << " " << duration << " " << tput << " " << avgTput << " " << bitRate << std::endl; 

	file.close(); 

	return; 
}

#endif
