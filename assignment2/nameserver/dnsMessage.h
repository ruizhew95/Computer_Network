#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "DNSHeader.h"
#include "DNSQuestion.h"
#include "DNSRecord.h"


//Functions to create DNS Headers & Messages
 
//Returns encoded string for a header for a QUERY
std::string headerQuery(ushort id){

	DNSHeader head;
	head.ID = id; 
	//QR == 0 for Query
	head.QR = 0; 
	head.OPCODE = 0; 
	head.AA = 0; 
	head.TC = 0; 
	head.RD = 0; 
	head.RA = 0; 
	head.Z = '0'; 
	head.RCODE = '0';
	head.QDCOUNT = 0; 
	head.ANCOUNT = 0; 
	head.NSCOUNT = 0;  
	head.ARCOUNT = 0; 
	
	

	std::string header = DNSHeader::encode(head); 
		

	return header;
}

//Returns encoded string for a header for a RESPONSE
std::string headerResponse(ushort id){

	DNSHeader head;
	head.ID = id; 
	//QR == 1 for Response 
	head.QR = 1; 
	head.OPCODE = 0; 
	head.AA = 1; 
	head.TC = 0; 
	head.RD = 0; 
	head.RA = 0; 
	head.Z = '0'; 
	head.RCODE = '0';
	head.QDCOUNT = 0; 
	head.ANCOUNT = 0; 
	head.NSCOUNT = 0;  
	head.ARCOUNT = 0; 
	
	

	std::string header = DNSHeader::encode(head); 
		

	return header;
}


//Returns encoded string of DNS Question
std::string question(std::string domain){
	
	DNSQuestion q; 

	q.QNAME[0] = char(domain.length()); 
	for (int i = 0; i < int(domain.length()); i++){
		
		q.QNAME[i + 1] = domain[i]; 
	}


	q.QTYPE = 1; 
	q.QCLASS = 1; 
	
	std::string toReturn = DNSQuestion::encode(q); 

	return toReturn; 
}

//Returns string of encoded DNS response
std::string response(std::string domain, std::string ip){

	DNSRecord r; 
	for (int i = 0; i < int(domain.length()); i++){
		r.NAME[i] = domain[i]; 
	} 

	r.TYPE = 1; 
	r.CLASS = 1; 
	r.TTL = 0; 
	r.RDLENGTH = ushort(ip.length()); 
	
	for (int i = 0; i < int(ip.length()); i++){
		r.RDATA[i] = ip[i]; 
	} 
	
	std::string toReturn = DNSRecord::encode(r); 

	return toReturn; 
	
}


//requires you've got an encoded dnsHeader string. Handle the ID too. 
std::string sendHeader(int sock, std::string header) {
	
	//Send integer designating size of DNS header.
	uint32_t size = uint32_t(header.length()); 
	
	//change endian-ness (host to network)
	size = htonl(size); 

	std::cout << size << std::endl; 

	//USE MEMCPY here to put size into a buffer?
	send(sock, size, 4, MSG_NOSIGNAL); 

	send(sock, &header[0], size, MSG_NOSIGNAL); 
	
	return "uh"; 
}









#endif
