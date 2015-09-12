#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#if 0
/* 
 * Structs exported from netinet/in.h (for easy reference)
 */

/* Internet address */
struct in_addr {
  unsigned int s_addr; 
};

/* Internet style socket address */
struct sockaddr_in  {
  unsigned short int sin_family; /* Address family */
  unsigned short int sin_port;   /* Port number */
  struct in_addr sin_addr;	 /* IP address */
  unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};

/*
 * Struct exported from netdb.h
 */

/* Domain name service (DNS) host entry */
struct hostent {
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* length of address */
  char    **h_addr_list;  /* list of addresses */
}
#endif

#define BUFSIZE 4096

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoserver [options]\n"                                                    \
"options:\n"                                                                  \
"  -p                  Port (Default: 8888)\n"                                \
"  -n                  Maximum pending connections\n"                         \
"  -h                  Show this help message\n"                              

int main(int argc, char **argv) {
  int option_char;
  int portno = 8888; /* port to listen on */
  int maxnpending = 5;
  
  // Parse and set command line arguments
  while ((option_char = getopt(argc, argv, "p:n:h")) != -1){
    switch (option_char) {
      case 'p': // listen-port
        portno = atoi(optarg);
        break;                                        
      case 'n': // server
        maxnpending = atoi(optarg);
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

  int socket_fd = 0;
  int client_socket_fd = 0;
  
  char buffer[BUFSIZE];
  int num_bytes = 0;

  int set_reuse_addr = 1; // ON == 1  
  int max_pending_connections = maxnpending;

  struct sockaddr_in server;
  struct sockaddr_in client;
  struct hostent *client_host_info;
  char *client_host_ip;
  socklen_t client_addr_len = sizeof(client); 

bzero(&server, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(portno);

	while(1){

		if (0 > (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
		fprintf(stderr, "server failed to create the listening socket\n");
		exit(1);}
		if (0 != setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, 
		sizeof(set_reuse_addr))) {
		fprintf(stderr, "server failed to set SO_REUSEADDR socket option (not fatal)\n");
		exit(1);}

		if (0 > bind(socket_fd, (struct sockaddr *)&server, sizeof(server))) {
		fprintf(stderr, "server failed to bind\n");
		exit(1);}

		if (0 > listen(socket_fd, max_pending_connections)) {
		fprintf(stderr, "server failed to listen\n");
		exit(1);} 
		else {
		fprintf(stdout, "server listening for a connection on port %d\n", portno);}


		if (0 > (client_socket_fd = accept(socket_fd, (struct sockaddr *)&client, &client_addr_len))) 
		{fprintf(stderr, "server accept failed\n");exit(1);} 
		else {fprintf(stdout, "server accepted a client!\n");}

		client_host_info = gethostbyaddr((const char *)&client.sin_addr.s_addr, 
		sizeof(client.sin_addr.s_addr), AF_INET);
		if (client_host_info == NULL) {
		fprintf(stderr, "server could not determine client host address\n");
		} 
		client_host_ip = inet_ntoa(client.sin_addr);
		if (client_host_ip == NULL) {
		fprintf(stderr, "server could not determine client host ip\n");
		}
		fprintf(stdout, "server established connection with %s (%s)\n", 
		client_host_info->h_name, client_host_ip);
	    
		bzero(buffer, BUFSIZE);
		num_bytes = read(client_socket_fd, buffer, BUFSIZE);
		if (num_bytes == 0) {
		fprintf(stderr, "server could not read from socket\n");
		} else {
		fprintf(stdout, "server received %d bytes: %s\n", num_bytes, buffer);
		}

		if (0 > write(client_socket_fd, buffer, strlen(buffer))) {
		fprintf(stderr, "server could not write back to socket\n");
		} else {
		fprintf(stdout, "server sending message back to client\n");
		}
	
		close(socket_fd);
	}

  return 0;

}
