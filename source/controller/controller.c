#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <term.h>
#include <termios.h>
#include <pthread.h>
#include "../carData.h"
#include "../sigData.h"

//network define
#define DATAPORT 4004
#define REQUESTPORT 4005
#define BUFFSIZE 5000
#define IPSERVER "127.0.0.1"
#define IDCTLR 366

void *recv_rData(void *);
void send_rData();

char sh_b_send = 0;
char sh_buff[BUFFSIZE];


int getch() {
	int ch;
	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}

int main(int argc, char *argv[])
{
	//var network
	int clntSock;
	struct sockaddr_in servAddr;
	char *servIP;
	//var test
	int chkEnter;

	if (argc != 2) {
		/*fprintf(stderr, "Usage : %s <Server IP> <Port> \n", argv[0]);
		exit(1);*/
		servIP = "127.0.0.1";
	}
	else {
		servIP = argv[1];
	}

	if ((clntSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket Failed\n");

	pthread_t thID_recvrData;

	if (pthread_create(&thID_recvrData, NULL, recv_rData, NULL) < 0) {
		perror("create error: thread for reciving raw data\n");
		exit(1);
	}

	//servAddr init & set
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(DATAPORT);

	while (1) {
		puts("press enter key");
		chkEnter = getch();

		if (chkEnter == 10)
			if (sendto(clntSock, (char *)&cData, sizeof(carData), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != sizeof(carData))
				printf("sendto failed");

		chkEnter = 0;
	}

	return 0;
}

void *recv_rData(void *p) {
	//var network
	struct sockaddr_in cntlrAddr, carAddr;
	unsigned int carAddrLen;
	char buf[BUFFSIZE];
	int servSock;
	int clntSock;
	int end;

	//create sock
	if ((servSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket() failed");
	//init cntlrAddr & set cntlrAddr
	memset(&cntlrAddr, 0, sizeof(cntlrAddr));
	cntlrAddr.sin_family = AF_INET;
	cntlrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cntlrAddr.sin_port = htons(DATAPORT);
	if (bind(servSock, (struct sockaddr *) &cntlrAddr, sizeof(cntlrAddr)) < 0)
		puts("bind failed");

	carAddrLen = sizeof(carAddr);
	while (1) {

		if ((end = recvfrom(servSock, buf, BUFFSIZE - 1, 0, (struct sockaddr*)&carAddr, &carAddrLen)) < 0)
			printf("recvfrom failed\n");
		buf[end] = '\0';

		if (buf[0] != '\0') {
			memcpy(sh_buff, buf, BUFFSIZE);
			sh_b_send = 1;

			buf[0] = '\0';
		}
		usleep(10);
	}

	pthread_exit(NULL);
}

void *send_rData() {
	//var network
	int sock_sendrData;
	struct sockaddr_in servAddr;
	//var data
	sigSt data;

	if ((sock_sendrData = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket Failed\n");
	
	//servAddr init & set
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPSERVER);
	servAddr.sin_port = htons(DATAPORT);

	while (1) {
		if (sh_b_send == 1) {
			data.Car = (carData *)sh_buff;
			data.id = IDCTLR;
			//sig_num 판별 함수
			
			if (sendto(sock_sendrData, (char *)&cData, sizeof(carData), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != sizeof(carData))
				printf("sendto failed");
		} else {

		}
	}
}