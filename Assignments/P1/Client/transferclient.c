#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 4096

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  transferclient [options]\n"                                                \
"options:\n"                                                                  \
"  -s                  Server (Default: localhost)\n"                         \
"  -p                  Port (Default: 8888)\n"                                \
"  -o                  Output file (Default foo.txt)\n"                       \
"  -h                  Show this help message\n"                              

/* Main ========================================================= */
int main(int argc, char **argv) {
	int option_char = 0;
	char *hostname = "localhost";
	unsigned short portno = 8888;
	char *filename = "foo.txt";

	// Parse and set command line arguments
	while ((option_char = getopt(argc, argv, "s:p:o:h")) != -1) {
		switch (option_char) {
			case 's': // server
				hostname = optarg;
				break; 
			case 'p': // listen-port
				portno = atoi(optarg);
				break;                                        
			case 'o': // filename
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
	char revbuf[BUFSIZE]; 
	struct sockaddr_in remote_addr;
	
	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);exit(1);}
	
	/* Fill the socket address struct */
	
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(portno); 
	inet_pton(AF_INET, hostname, &remote_addr.sin_addr); 
	bzero(&(remote_addr.sin_zero), sizeof(remote_addr));
	
	
	/* Try to connect the remote */
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	{fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);exit(1);}
	else {printf("[Client] Connected to server at port %d...ok!\n", portno);}
	
	  
	char* cwd;
	char buff[BUFSIZE/2];
	cwd = getcwd( buff, BUFSIZE +1);
	strcat(strcat(cwd,"/"),filename);
	
	printf("[Client] Receiveing file from Server and saving it...\n");
	char* fr_name = cwd;
	FILE *fr = fopen(fr_name, "a");
	//int fr = open(fr_name, O_CREAT | O_TRUNC, S_IRUSR);
	
	if(fr == NULL)
	{fprintf(stderr, "File %s Cannot be opened.(errno = %d)\n", fr_name, errno);exit(1);}      
	else
	{
	  bzero(revbuf, BUFSIZE); 
	  int fr_block_sz = 0;
	  int br =0;
	    while((fr_block_sz = recv(sockfd, revbuf, BUFSIZE, 0)) > 0)
	    {
		br = br + (int)fr_block_sz;
		int write_sz = fwrite(revbuf, sizeof(char), recv(sockfd, revbuf, BUFSIZE, 0), fr);
	        if(write_sz < fr_block_sz)
		{fprintf(stderr,"File write failed. (errno = %d\n",errno);close (sockfd);exit(1);}
		//bzero(revbuf, BUFSIZE);
		
		if (fr_block_sz == 0 || fr_block_sz != BUFSIZE) 
		break;
		
		
	    }
	    
	    
	    printf("File %s received OK from server! and saved \n", filename);
	    printf("Total Bits Received: %d\n", br);
	    
	}
	fclose(fr);
	close (sockfd);
	printf("[Client] Connection lost.\n");
	return (0);
}
