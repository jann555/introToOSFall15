#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

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
#define BACKLOG 5

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  transferserver [options]\n"                                                \
"options:\n"                                                                  \
"  -p                  Port (Default: 8888)\n"                                \
"  -f                  Filename (Default: bar.txt)\n"                         \
"  -h                  Show this help message\n"                              

int main(int argc, char **argv) {
  int option_char;
  int portno = 8888; /* port to listen on */
  char *filename = "bar.txt"; /* file to transfer */

  // Parse and set command line arguments
  while ((option_char = getopt(argc, argv, "p:f:h")) != -1){
    switch (option_char) {
      case 'p': // listen-port
        portno = atoi(optarg);
        break;                                        
      case 'f': // listen-port
        filename = optarg;
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
	int sockfd; 
	int nsockfd; 
	int num;
	int sin_size; 
	
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	sin_size = sizeof(struct sockaddr_in);
	char revbuf[BUFSIZE]; // Receiver buffer

	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		exit(1);
	}
	else {
		printf("[Server] Obtaining socket descriptor successfully.\n");}

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Family
	addr_local.sin_port = htons(portno); // Port number
	addr_local.sin_addr.s_addr = htonl(INADDR_ANY); // AutoFill local address
	bzero(&(addr_local.sin_zero), sizeof(addr_local)); // Flush the rest of struct
	
	if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,  &(int){ 1 }, sizeof(int)) ==-1)
	{
        fprintf(stderr, "ERROR: setsockopt() (errno = %d)\n", errno);
		exit(1);
	}
	
	/* Bind a special Port */
	if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Binded tcp port %d sucessfully.\n",portno);

	/* Listen remote connect/calling */
	if(listen(sockfd,BACKLOG) == -1)
	{
		fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
		exit(1);
	}
	else{
		printf ("[Server] Listening the port %d successfully.\n", portno);}
		
	
	while(1)
	{
		

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
		    fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else {
			printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));}
		  
		    char* cwd;
		    char buff[BUFSIZE/2];
		    cwd = getcwd( buff, BUFSIZE +1);
		
		    char* fs_name = strcat(strcat(cwd,"/"),filename);
		    char sdbuf[BUFSIZE]; // Send buffer
		    printf("[Server] Sending %s to the Client...\n", fs_name);
		    FILE *fs = fopen(fs_name, "r");
		    if(fs == NULL)
		    {
		        fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
			close(nsockfd);
			exit(1);
		    }

		    bzero(sdbuf, BUFSIZE); 
		    size_t fs_block_sz;
		    ssize_t sent;
		    int bt;
		    while((fs_block_sz = fread(sdbuf, sizeof(char), BUFSIZE, fs))>0)
		    {
		        if(0> send(nsockfd, sdbuf, fs_block_sz, 0))
		        {
		            fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
			    fclose(fs);
			    close(nsockfd);
		            exit(1);
		        }
		       bt = bt+ (int)fs_block_sz;
		       bzero(sdbuf, BUFSIZE);
		    }
		    
		    printf("Ok sent to client!\n");
		    fclose(fs);
		    
		    printf("[Server] Connection with Client closed. Data Sent BS:[%d].\n Server will wait now...\n",bt);
		   // while(waitpid(-1, NULL, WNOHANG) > 0);
		//}
	}
	close(nsockfd);
	return (0);

}