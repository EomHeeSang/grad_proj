#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "carData.h"

#define PORT 7777   /* port */
#define BUFSIZE 10240

int main() {
	int sockfd;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	char recvBuffer[BUFSIZE];
	int clntLen;
	int recvLen;

	/* create UDP socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("sock failed");
		exit(1);
	}

	/* init servAddr */
	memset(&servAddr, 0, sizeof(servAddr));
	/* set servAddr */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(PORT);

	/* bind sockfd & servAddr */
	if (bind(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
		perror("bind failed");
		exit(1);
	}

	while (1) {
		clntLen = sizeof(clntAddr);
		/* write data from client on recvBuffer
		write client's info on clntAddr */
		if ((recvLen = recvfrom(sockfd, recvBuffer, BUFSIZE - 1, 0, (struct sockaddr*)&clntAddr, &clntLen)) == -1) {
			perror("recvfrom failed");
			exit(1);
		}
		recvBuffer[recvLen] = '\0';
		
		carData *cData = (carData *)recvBuffer;

		/* data check */
		printf("speed: %d\n", cData->speed);

		/* send back to client for checking integrity */
		if (sendto(sockfd, (char *)recvBuffer, sizeof(carData) , 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr)) != recvLen) {
			perror("sendto failed");
			exit(1);
		}
	}
}