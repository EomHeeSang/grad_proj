#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "carData.h"

#define PORT 7777   /* server's port */
#define BUFSIZE 10240

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servAddr;
	char *sendBuffer, recvBuffer[BUFSIZE];
	int recvLen, servLen;

	char *ip;

	if (argc != 2) {
		/*fprintf(stderr, "Usage: %s IP_address\n", argv[0]);
		exit(1);*/
		ip = "127.0.0.1";
	}
	ip = argv[1];

	/* create socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("sock failed");
		exit(1);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	/* set servAddr */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(PORT);

	carData cData;
	cData.id = 339;
	cData.lane_num = 2;
	cData.curX = 100;
	cData.curY = 200;
	cData.speed = 85;
	cData.distance = 20;
	cData.emergency = EMERGENCY;
	cData.direction = 'l';
	cData.time = time((time_t *)0);

	while (1) {
		resend:
		/* send data */
		if (sendto(sockfd, (char *)&cData, sizeof(carData), 0, (struct sockaddr*)&servAddr, sizeof(servAddr)) != sizeof(carData)) {
			perror("sendto failed");
			exit(1);
		}
		sleep(1);

		servLen = sizeof(servLen);
		/* data check to resend */
		if ((recvLen = recvfrom(sockfd, recvBuffer, BUFSIZE - 1, 0, (struct sockaddr*)&servAddr, &servLen)) != sizeof(carData)) {
			perror("recvfrom failed");
			goto resend;
		}
		printf("success\n");
	}
	close(sockfd);
	exit(0);
}