#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define	SA	struct sockaddr
#define	LISTENQ		1024	/* 2nd argument to listen() */
#define	MAXLINE		4096	/* max text line length */
//helper functions 

//code from the original files given by the textbooks (for use if need be)
//if ( (childpid = fork()) == 0) { /*child process*/
			//close(listenfd); /*close listening socket*/
			//str_echo(connfd); /* process the request */
			//exit (0);
		//}

int main(int argc, char **argv)
{
	int listenfd, connfd;
	int n;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	char buffer[MAXLINE];
	char recvline[MAXLINE+1];
	FILE *fp;
	char path[MAXLINE];
	
	if(argc < 2){
		printf("usage: filename, PORT_NUM \n");
		exit(0);
	}
	
 	listenfd = socket (AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (atoi(argv[1]));
	if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr))< 0){
		printf("error:could not bind server");
		exit(1);
	}
	
	listen(listenfd, LISTENQ);
	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
		if (connfd <0){
			printf("error:could not accept connection.");
		}
		
		while ((n = read(connfd, recvline, MAXLINE))>0){
			recvline[n] = 0; 
			
			//this is to run the  commands
			fp = popen(recvline,"r");
			if(fp == NULL) {
				perror("Error: ");
				return (-1);
				
			}
			while (fgets(path,MAXLINE,fp) != NULL){
				strncat(buffer,path,strlen(path));
			}
			//bzero(buffer,strlen(buffer));
			//print buffer to string and to output 
			printf("%s",buffer);
			//write this buffer to the connected client 
			if(write(connfd,buffer,strlen(buffer))<0){
				printf("error: could not write to client");
				exit(1);
			}
			//zero out the buffer to accept new feedback
     		bzero(buffer,strlen(buffer));
			//close the file.
    		pclose(fp);
		}
		
		close(connfd); /* parent closes connected socket */
	}
 }
