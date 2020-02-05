#include <arpa/inet.h>		// htons()
#include <stdio.h>		// printf(), perror()
#include <stdlib.h>		// atoi()
#include <sys/socket.h>		// socket(), bind(), listen(), accept(), send(), recv()
#include <unistd.h>		// close()

#include "helpers.h"		// make_server_sockaddr(), get_port_number()

static const int MAX_MESSAGE_SIZE = 256;

/**
 * Receives a null-terminated string message from the client, prints it to stdout, 
 * then sends the integer 42 back to the client as a success code.
 *
 * Parameters:
 * 		connectionfd: 	File descriptor for a socket connection (e.g. the one
 *				returned by accept())
 * Returns:
 *		0 on success, -1 on failure.
 */
int handle_connection(int connectionfd) {

	printf("New connection %d\n", connectionfd);

	// (1) Receive message from client.
	char msg[MAX_MESSAGE_SIZE];
	memset(msg, 0, sizeof(msg));

	for (int i = 0; i < MAX_MESSAGE_SIZE; i++) {
		// Receive exactly one byte
		int rval = recv(connectionfd, msg + i, 1, MSG_WAITALL);
		if (rval == -1) {
			perror("Error reading stream message");
			return -1;
		}
		// Stop if we received a null character
		if (msg[i] == '\0') {
			break;
		}
	}

	// (2) Print out the message
	printf("Client %d says '%s'\n", connectionfd, msg);

	// (3) Send response code to client
	int response = 42;
	response = htons(response);
        if (send(connectionfd, &response, sizeof(response), 0) == -1) {
                perror("Error sending response to client");
                return -1;
        }


	// (4) Close connection
	close(connectionfd);

	return 0;
}

/**
 * Endlessly runs a server that listens for connections and serves
 * them _synchronously_.
 *
 * Parameters:
 *		port: 		The port on which to listen for incoming connections.
 *		queue_size: 	Size of the listen() queue
 * Returns:
 *		-1 on failure, does not return on success.
 */
int run_server(int port, int queue_size) {

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
	port = get_port_number(sockfd);
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

		if (handle_connection(connectionfd) == -1) {
			return -1;
		}
	}
}

int main(int argc, const char **argv) {
	// Parse command line arguments
	if (argc != 2) {
		printf("Usage: ./server port_num\n");
		return 1;
	}
	int port = atoi(argv[1]);

	if (run_server(port, 10) == -1) {
		return 1;
	}
	return 0;
}
