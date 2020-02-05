#include <arpa/inet.h>		// ntohs()
#include <stdio.h>		// printf(), perror()
#include <stdlib.h>		// atoi()
#include <string.h>		// strlen()
#include <sys/socket.h>		// socket(), connect(), send(), recv()
#include <unistd.h>		// close()

#include "helpers.h"		// make_client_sockaddr()

static const int MAX_MESSAGE_SIZE = 256;

/**
 * Sends a string message to the server and waits for an integer response.
 *
 * Parameters:
 *		hostname: 	Remote hostname of the server.
 *		port: 		Remote port of the server.
 * 		message: 	The message to send.
 * Returns:
 *		The server's response code on success, -1 on failure.
 */
int send_message(const char *hostname, int port, const char *message) {
	if (strlen(message) > MAX_MESSAGE_SIZE) {
		perror("Error: Message exceeds maximum length\n");
		return -1;
	}

	int response = -1;

	// (1) Create a socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// (2) Create a sockaddr_in to specify remote host and port
	struct sockaddr_in addr;
	if (make_client_sockaddr(&addr, hostname, port) == -1) {
		return -1;
	}

	// (3) Connect to remote server
	if (connect(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
		perror("Error connecting stream socket");
		return -1;
	}
	
	// (4) Send message to remote server
	if (send(sockfd, message, strlen(message) + 1, 0) == -1) {
		perror("Error sending on stream socket");
		return -1;
	}

	// (5) Wait for integer response
	if (recv(sockfd, &response, sizeof(response), MSG_WAITALL) == -1) {
		perror("Error receiving response from server");
		return -1;
	}

	// Convert from network to host byte order
	response = ntohs(response);

	// (6) Close connection
	close(sockfd);

	return response;
}

int main(int argc, const char **argv) {
	// Parse command line arguments
	if (argc != 4) {
		printf("Usage: ./client hostname port_num message\n");
		return 1;
	}
	const char *hostname = argv[1];
	int port = atoi(argv[2]);
	const char *message = argv[3];
	printf("Sending message %s to %s:%d\n", message, hostname, port);

	int response = send_message(hostname, port, message);
	if (response == -1) {
		return 1;
	}
	printf("Server responds with status code %d\n", response);
	return 0;
}
