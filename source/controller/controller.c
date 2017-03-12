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
#define REQUESTPORT 5005
#define SERVERDATAPORT 5004
#define BUFFSIZE 5000
#define IPSERVER "127.0.0.1"
#define IDCTLR 366

void *recv_rData(void *);
void send_CtData(char *);

char sh_b_send = 0;
char sh_buff[BUFFSIZE];

int main(int argc, char *argv[]){
	char* servIP;

	if (argc != 2) {
		/*fprintf(stderr, "Usage : %s <Server IP> <Port> \n", argv[0]);
		exit(1);*/
		servIP = "127.0.0.1";
	}
	else {
		servIP = argv[1];
	}

	pthread_t thID_recvrData;
	//pthread_t thID_sendCtlrData;

	if (pthread_create(&thID_recvrData, NULL, recv_rData, NULL) < 0) {
		perror("create error: thread for reciving raw data\n");
		exit(1);
	}
	/*if (pthread_create(&thID_sendCtlrData, NULL, send_CtData, NULL) < 0) {
		perror("create error: thread for sending data\n");
		exit(1);
	}*/

	while (1) {
		sleep(10);
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
			send_CtData(buf);
			//memcpy(sh_buff, buf, BUFFSIZE);
			//sh_b_send = 1;
			printf("data arrived\n");
			buf[0] = '\0';
		}
		usleep(10);
	}

	pthread_exit(NULL);
}

void send_CtData(char *buf) {
	//var data
	sigSt st_sig;
	carData *temp_cardata;
	char buff[BUFFSIZE] = { 0, };
	//var network
	static int sock_sendrData;
	struct sockaddr_in servAddr;

	//if method called first time
	if (servAddr.sin_addr.s_addr != inet_addr(IPSERVER)) {
		if ((sock_sendrData = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			puts("socket Failed\n");

		//servAddr init & set
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr(IPSERVER);
		servAddr.sin_port = htons(SERVERDATAPORT);
	}

	//car data + signal data
	/*temp_cardata = (carData *)buff;
	st_sig.Car = *temp_cardata;*/
	st_sig.id = IDCTLR;
	//sig_num 판별 함수
	st_sig.gSig.sig_num = 1;
	st_sig.gSig.sig_value = 2;

	strcat(buff, (char *)st_sig.id);
	strcat(buff, (char *)st_sig.gSig);
	strcat(buff, buf);
			
	if (sendto(sock_sendrData, (char *)buff, BUFFSIZE, 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != BUFFSIZE)
		printf("sendto failed");	
}

void *send_Request() {
	//var network
	int sock_sendReq;
	struct sockaddr_in servAddr;
	//var data
	char *request ="0";

	if ((sock_sendReq = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket Failed\n");

	//servAddr init & set
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPSERVER);
	servAddr.sin_port = htons(REQUESTPORT);

	while (1) {
		if (sh_b_send == 1) {
			if (sendto(sock_sendReq, (char *)request, sizeof(sigSt), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != sizeof(sigSt))
				printf("sendto failed");

		}
		else {

		}
		usleep(50);
	}
}