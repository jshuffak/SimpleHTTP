/*
	Jordan Huffaker - 0026480616 - jhuffak@purdue.edu
	ECE 463 Lab 1
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
//#include <usistd.h>

#define MAXLINE 300

int open_clientfd(char *hostname, int port);

// CREDIT: Sanjay Rao - Lecture Socket Programming slide 14
int main(int argc, char **argv)
{
	int clientfd, port;
	char *host, buf[MAXLINE], temp[20];

	host = argv[1];
	port = atoi(argv[2]);
	clientfd = open_clientfd(host, port);
	
	strcpy(buf, "GET ");
	strcat(buf, argv[3]);
	strcpy(temp, " HTTP/1.0\r\n\r\n\0");
	strcat(buf, temp);
	
	write(clientfd, buf, strlen(buf));
	while (read(clientfd, buf, sizeof(char)) != 0) {
		fputc(*buf, stdout);
	}
	close(clientfd);
	exit(0);
}

// CREDIT: Sanjay Rao - Lecture Socket Programming slide 15
int open_clientfd(char *hostname, int port)
{
	int clientfd;
	struct hostent *hp;
	struct sockaddr_in serveraddr;
	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1; /* check errno for cause of error */
	/* Fill in the server's IP address and port */
	if ((hp = gethostbyname(hostname)) == NULL)
		return -2; /* check h_errno for cause of error */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)hp->h_addr,
		(char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);
	/* Establish a connection with the server */
	if (connect(clientfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
		return -1;
	return clientfd;
}
