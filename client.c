#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

      		int sockfd,n;
				struct addrinfo hints, *servinfo, *p;
				int rv;
                char buffer[256],temp[256];
    			if (argc < 3) {
       			fprintf(stderr,"usage %s hostname port\n", argv[0]);
     		    exit(0);
  				  }
				memset(&hints, 0, sizeof hints);
				hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
				hints.ai_socktype = SOCK_STREAM;

				if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
					fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
					exit(0);
				}

				// loop through all the results and connect to the first we can
				for(p = servinfo; p != NULL; p = p->ai_next) {
					if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
						perror("socket");
						continue;
				}

				if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
					close(sockfd);
					perror("connect");
					continue;
				}

				break; // if we get here, we must have connected successfully
				}

				if (p == NULL)
				{
					// looped off the end of the list with no connection
					fprintf(stderr, "failed to connect\n");
					exit(0);
				}
    			printf("REQUEST: \n");
    			bzero(buffer,256);
  				bzero(temp,256);
   		 		fgets(buffer,255,stdin);
   			 	fgets(temp,255,stdin);
    			strcat(buffer," ");
   				strcat(buffer,temp);
   				n = write(sockfd,buffer,strlen(buffer));
   				if (n < 0) 
     		    error("ERROR writing to socket");
    			bzero(buffer,256);
   				n = read(sockfd,buffer,255);
    			if (n < 0) 
      			error("ERROR reading from socket");
    			printf("%s\n",buffer);
    			close(sockfd);
   				return 0;
}
