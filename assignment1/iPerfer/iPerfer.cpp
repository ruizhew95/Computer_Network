#include <arpa/inet.h>		// htons(), ntohs()
#include <netdb.h>		// gethostbyname(), struct hostent
#include <netinet/in.h>		// struct sockaddr_in
#include <stdlib.h>		// atoi()
#include <stdio.h>		// perror(), fprintf()
#include <string.h>		// memcpy()
#include <sys/socket.h>		// getsockname()
#include <unistd.h>		// stderr
#include <sys/time.h>       //gettimeofday

int make_server_sockaddr(struct sockaddr_in *addr, int port) {
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(port);

	return 0;
}


int run_as_server(int port, int queue_size){
    // (1) Create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error opening stream socket");
		return -1;
	}

	// (2) Set the "reuse port" socket option
	int yesval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yesval, sizeof(yesval)) == -1) {
		perror("Error setting socket options");
		return -1;
	}

	// (3) Create a sockaddr_in struct for the proper port and bind() to it.
	struct sockaddr_in addr;
	if (make_server_sockaddr(&addr, port) == -1) {
		return -1;
	}
	if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("Error binding stream socket");
		return -1;
	}
	
    // (3b) Detect which port was chosen
	socklen_t length = sizeof(addr);
	if (getsockname(sockfd, (sockaddr *) &addr, &length) == -1) {
		perror("Error getting port of socket");
		return -1;
	}
    port = ntohs(addr.sin_port); 
	printf("Server listening on port %d...\n", port);

	// (4) Begin listening for incoming connections.
	listen(sockfd, queue_size);

	// (5) Serve incoming connections one by one forever.
	while (true) {
		int connectionfd = accept(sockfd, 0, 0);
		if (connectionfd == -1) {
			perror("Error accepting connection");
			return -1;
		}

    	else
        {
            printf("New connection %d\n", connectionfd);

        	// (1) Receive message from client.
        	char msg[1000];
        	memset(msg, 0, sizeof(msg));
            int len = 1000;
            int ReceivedBytes = 0;

            //track the time
            struct timeval tv1, tv2;
            gettimeofday(&tv1, NULL);
        	while(1) {
                // FIN message is a @ character
                recv(connectionfd, msg, 1, MSG_PEEK);
			// for debugging
			printf("Executed 81 here=%s\n", msg);
			if (strcmp(msg, "@") == 0) {
        		// for debugging
			printf("enter line 83\n");
                    //for debugging
                    printf("Here at 82");
                    printf("Executed 81 here=%s", msg);
        		if (strcmp(msg, "@")==0) {
        			if(send(connectionfd, "FIN", 3, 0) == -1){
                        perror("Error: Wrong with response");
                    }
                    gettimeofday(&tv2, NULL);
                    break;
        		}
        		// Receive exactly one byte
			//for debugging
			printf("Executed 94 here\n");
        		//int rval = recv(connectionfd, msg, len, MSG_WAITALL);
        		int rval = recv(connectionfd, msg, 10, MSG_WAITALL);
        		if (rval == -1) {
        			perror("Error reading stream message");
        			return -1;
        		}
                else ReceivedBytes += rval;
   		printf("ReceivedBytes=%d\n",ReceivedBytes); 
        	}

        	close(connectionfd);

            double time = tv2.tv_sec - tv1.tv_sec;
            time += (tv2.tv_usec - tv1.tv_usec)/1000000;
		//for debuging
	    printf("Size in bytes=%d B, Time=%.3f",ReceivedBytes,time);
            int ReceivedKBytes = ReceivedBytes/1000;    //data size: KB
            double rate = 8 * (ReceivedKBytes/time)/1000; //rate: Mbps
            //for debugging
        	printf("ReceivedBytes=%d B, Time=%.3f s\n",ReceivedBytes, time);
        	printf("Received=%d KB, Rate=%.3f Mbps\n",ReceivedKBytes, rate);
            return 0;
        }
    }
    
}

int make_client_sockaddr(struct sockaddr_in *addr, const char *hostname, int port) {
	addr->sin_family = AF_INET;
	struct hostent *host = gethostbyname(hostname);
	if (host == nullptr) {
		fprintf(stderr, "%s: unknown host\n", hostname);
		return -1;
	}
	memcpy(&(addr->sin_addr), host->h_addr, host->h_length);
	addr->sin_port = htons(port);

	return 0;
}



int run_as_client(const char *hostname, int port, int time) {
    
    // (1) Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // (2) Create a sockaddr_in to specify remote host and port
    struct sockaddr_in addr;
    if (make_client_sockaddr(&addr, hostname, port) == -1) {
    	return -1;
    }

    // (3) Connect to remote server
    if (connect(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
    	perror("Error: connecting stream socket");
    	return -1;
    }
    
    // (4) Send message to remote server
    char msg[1000];
    memset(msg, 0, sizeof(msg));
    int len = 1000;
    
    double MeasureTime;
    int SentKBytes = 0;

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    while(MeasureTime < time){

    
        if (send(sockfd, msg, len, 0) == -1) {
        	perror("Error: sending on stream socket");
        	return -1;
        }
        SentKBytes++;
    }

    if(send(sockfd, "@", 1, 0)== -1){
        perror("Error: sending FIN");
        return -1;
    }

    char Ack[100];
    int Alen = 100;
    if(recv(sockfd, Ack, Alen, 0) < 0){
        perror("Error: receiving Ack");
    }

    if(strcmp("FIN", Ack)==0){
        gettimeofday(&tv2,NULL);
        MeasureTime = tv2.tv_sec - tv1.tv_sec;
        MeasureTime += (tv2.tv_usec - tv1.tv_usec)/1000000;
        double rate = 8 * (SentKBytes/MeasureTime)/1000;
        printf("Sent=%d KB, Rate=%.3f Mbps\n", SentKBytes, rate);
    }
        // (6) Close connection
    close(sockfd);
    return 0;
}

int main(int argc, char **argv){
   
    if(argc !=4 && argc !=8) {
       perror("Error: missing or extra arguments\n");
       return 0;
    }
    else{    
        if(strcmp(argv[1],"-s")==0){ //server mode
            if(argc != 4){
                perror("Error: missing or extra arguments\n");
                return 0;
            }
            else if ((atoi(argv[3])<1024) || (atoi(argv[3])>65535)){
                perror("Error: port number must be in the range of [1024, 65535]");
                return 0;
            }
            int port = atoi(argv[3]);
            if(run_as_server(port, 10) == -1){
                perror("Error: server running");
                return 0;
            }

         return 0;
        }
    }
     
        else if(strcmp(argv[1],"-c") == 0){//client mode
            if(argc != 8){
                perror("Error: missing or extra arguments\n");
                return 0;
            }
            else if ((atoi(argv[5])<1024) || (atoi(argv[5])>65535)){
                perror("Error: port number must be in the range of [1024, 65535]");
                return 0;
            }
            else if(argv[7] < 0){
                perror("Error: time argument must be greater than 0");
                return 0;
            }

            const char *hostname = argv[3];
	        int port = atoi(argv[2]);
            int time = atoi(argv[7]);
            run_as_client(hostname, port, time);
            return 0; 
        } 

    }
}
