/*
   jitendra singh solanki
   roll_no=10010124
*/
/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void dostuff(int); /* function prototype */
void error(const char *msg)/*error check function*/
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {                      
         fprintf(stderr,"ERROR, no port provided\n");               /*argument checking*/
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);                       /*create new sockfd*/
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));                  //intialization with zero of all address
     portno = atoi(argv[1]);   
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;                        /*assign header*/
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,              /*bind the address*/
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);                                             /*listen to correspondng socketfd*/
     clilen = sizeof(cli_addr);                                    /*size of client address*/
     while (1) {                                              
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);             /*accept the address of client*/
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();                                             /*run till we didn't stop it manually ctrl+c every time it's make new thread for connection*/
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);                                
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
   char *port="80";                                                                              /*default port for host*/
   bzero(buffer,256);
   n = read(sock,buffer,255);                                                                    /*read from buffer*/
   if (n < 0) error("ERROR reading from socket");
   int i=0;
   char *firststr[4],*context,*url,*http,*token;                             
   int inputLength=strlen(buffer);                                       
   char *inputCopy=(char *)calloc(inputLength+1,sizeof(char));
   strncpy(inputCopy,buffer,inputLength);
   for (token = strtok_r (inputCopy, " ", &context); token != NULL && i<4;                             /*parsing of client request address*/    
        token = strtok_r (NULL, " ", &context))
        {
  		firststr[i]= token;
			i++;
		}
	if(context!=NULL)
	{
	 		n = write(sock,"400 Bad Request",18);
			if (n < 0) error("ERROR writing to socket");
	}
   else if(strcmp(firststr[0],"GET")==0)                                                           /*CHECKING FOR GET*/
   {
	    
		http=strtok_r(firststr[1],"/",&context);
		if(strcmp(http,"http:")==0 && context!=NULL)                                              /*CHECKING FOR http*/
		{
				i=0;
				url=strtok_r(NULL,"/",&context);
				printf("%s",context);
				if(context!=NULL)
				{
					strcat(url,"/");
					context[strlen(context)-1]='\0';
					strcat(url,context);
				}
				int sockfd;
				struct addrinfo hints, *servinfo, *p;									/*make new socketfd for proxy server*/
				int rv;

				memset(&hints, 0, sizeof hints);                                      /*intializtion for connect host to proxy server*/
				hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
				hints.ai_socktype = SOCK_STREAM;

				if ((rv = getaddrinfo(url, port, &hints, &servinfo)) != 0) {
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
				char *temp;
				int n1=1,j=0;
				j = send(sock, temp,255,MSG_NOSIGNAL);
				if (j < 0) close(sockfd);
				while(n1!=0)															/*send and read data read from proxy server and send server to client*/
				{
					n1 = read(sockfd, temp,255);
					if (n1 < 0) error("ERROR writing to socket");
					j = send(sock, temp,255,MSG_NOSIGNAL);
					if (j < 0) close(sockfd);
				}
			    printf("Request by Client: %s",buffer);
			    freeaddrinfo(servinfo);
		}
		else if(http==NULL || (strcmp(http,"http:")==0 && context!=NULL))
		{
			if(firststr[3]!=NULL)
			{
				url=strtok_r(firststr[3],"Host: ",&context);
				int sockfd;
				struct addrinfo hints, *servinfo, *p;
				int rv;

				memset(&hints, 0, sizeof hints);
				hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
				hints.ai_socktype = SOCK_STREAM;

				if ((rv = getaddrinfo(url, port, &hints, &servinfo)) != 0) {
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

				freeaddrinfo(servinfo);
				
				char *temp;
				int n1=1,j=0;
				j = send(sock, temp,255,MSG_NOSIGNAL);
				if (j < 0) close(sockfd);
				while(n1!=0)															/*send and read data read from proxy server and send server to client*/
				{
					n1 = read(sockfd, temp,255);
					if (n1 < 0) error("ERROR writing to socket");
					j = send(sock, temp,255,MSG_NOSIGNAL);
					if (j < 0) close(sockfd);
				}
				printf("Request by Client: %s",buffer);
			    freeaddrinfo(servinfo);
			}
			else 
			{
				n = write(sock,"400 Bad Request",18);
				if (n < 0) error("ERROR writing to socket");
			}
		}
		else
		{
			n = write(sock,"400 Bad Request",18);
			if (n < 0) error("ERROR writing to socket");	
		}						
   }
   else if(strcmp(firststr[0],"POST")==0 || strcmp(firststr[0],"PUT")==0 || strcmp(firststr[0],"HEAD")==0)                          /*we not implemanted*/
   {
	   n=write(sock,"501 Not Implemaented",22); 
	   if (n < 0) error("ERROR writing to socket");
   }
   else
   {
		n=write(sock,"400 Bad Request",22); 
		if (n < 0) error("ERROR writing to socket");
   }
   free(inputCopy);
}
