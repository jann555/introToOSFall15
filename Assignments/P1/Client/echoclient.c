#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>

/* Be prepared accept a response of this length */
#define BUFSIZE 4096

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoclient [options]\n"                                                    \
"options:\n"                                                                  \
"  -s                  Server (Default: localhost)\n"                         \
"  -p                  Port (Default: 8888)\n"                                \
"  -m                  Message to send to server (Default: \"Hello World!\"\n"\
"  -h                  Show this help message\n"                              

/* Main ========================================================= */
int main(int argc, char **argv) {
	int option_char = 0;
	char *hostname = "localhost";
	unsigned short portno = 8888;
	char *message = "Hello World!";

	// Parse and set command line arguments
	while ((option_char = getopt(argc, argv, "s:p:m:h")) != -1) {
		switch (option_char) {
			case 's': // server
				hostname = optarg;
				break; 
			case 'p': // listen-port
				portno = atoi(optarg);
				break;                                        
			case 'm': // server
				message = optarg;
				break;    
			case 'h': // help
				fprintf(stdout, "%s", USAGE);
				exit(0);
				break;       
			default:
				fprintf(stderr, "%s", USAGE);
				exit(1);
		}
	}

	  /* Socket Code Here */
	int sockfd =0;
	char sdbuf[BUFSIZE];
	struct sockaddr_in socket_addr;
	struct hostent *he = gethostbyname(hostname);
 	unsigned long server_addr_nbo = *(unsigned long *)(he->h_addr_list[0]);

	if( 0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		fprintf(stderr, "client failed to create socket\n");
		exit(1);
	}
	bzero(&socket_addr, sizeof(socket_addr));
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port =htons(portno);
	socket_addr.sin_addr.s_addr = server_addr_nbo;

	if (0 > connect(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr))) {
	fprintf(stderr, "client failed to connect to %s:%d!\n", hostname, portno);
	close(sockfd);
	exit(1);
	}
	else {
	fprintf(stdout, "client connected to to %s:%d!\n", hostname, portno);
	}

	if (0 > send(sockfd, message, strlen(message), 0)) {
	fprintf(stderr, "client failed to send echo message");
	close(sockfd);
	exit(1);
	}

	bzero(sdbuf, BUFSIZE);
	if(0 > read(sockfd, sdbuf, BUFSIZE)) {
	fprintf(stderr, "client could not read response from server\n");
	close(sockfd);
	exit(1);
	}
	else {
	 fprintf(stdout, "echo from server: %s\n", sdbuf);
	}
	
	close(sockfd);
	return 0; 

}
