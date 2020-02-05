#ifndef HEADER_H
#define HEADER_H


#include "dnsMessage.h"
#include "DNSHeader.h"
#include "DNSQuestion.h"
#include "DNSRecord.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>


void nameserver(char* argv[]) {
	
	int port = atoi(argv[2]); 
	int geo = 0; 

	//if RR, geo goes unchanged. If geo, =1 
	if (argv[1] == "--geo"){
		geo = 1; 		
	}


	//TODO
	//RECEIVING QUERIES
	//Start listening/creating for connections
	//Receive integer designating size of DNS header
	//Receive encoded DNS header
	//Process DNS Header
	//Receieve integer designating size of DNS Question
	//Receieve DNS Question
	//Process DNS Question
	
	//SENDING RESPONSE
	//Send integer designating size of DNS header
	//Send encoded DNS header
	//Send integer designating size of DNS record
	//Send encoded DNS record


	
	
	return; 
}







#endif
