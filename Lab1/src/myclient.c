#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <readline/readline.h>
#include <readline/history.h>

#define	SA	struct sockaddr
#define	MAXLINE		4096	/* max text line length */
 
/* void str_cli(FILE *fp, int sockfd)
 {
	char sendline[MAXLINE], recvline[MAXLINE];
	
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		write(sockfd, sendline, strlen (sendline));
		if (readline(recvline) == 0)
			exit(EXIT_FAILURE);
		fputs(recvline, stdout);
	}
 } */
 //str_cli(stdin, sockfd); /* do it all */



int main(int argc, char **argv)
 {
	int sockfd;
 	struct sockaddr_in servaddr;
	char rcvline[MAXLINE+1];
	char buffer[MAXLINE];
	
	if (argc < 3) {
		printf("myclient, IP ADDRESS, Port Number");
		exit(0);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("error could not connect socket");
		exit(1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) <0 ){
		printf("error could not connect to server\n");
	}
	for ( ; ; ){
		printf(" client $ \n");
		fgets(buffer,MAXLINE,stdin);
		
		//check for exit command if they match exit and close sockets
		if (strncmp(buffer, "exit",4) == 0){
			close(sockfd);
			printf ("Exit command received\n");
			return(0);
		}
		//write it to the server
		if(write(sockfd, buffer, strlen(buffer))<0){
		printf("error could not write to server");
		exit(1);
		}
		bzero(rcvline,MAXLINE);
		//read from the server
		if(read(sockfd,rcvline,MAXLINE)<0){
		printf("error could not read from server");
		exit(1);
		}
		printf("%s\n",rcvline);
		
	}
	
	

	exit(0);
	}
