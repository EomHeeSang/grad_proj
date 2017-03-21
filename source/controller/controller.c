#include <stdio.h>
#include <math.h>
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
void makeSigSt(sigSt *);
void initSignal();
int gps_mapping(float, float);

char sh_b_send = 0;
char sh_buff[BUFFSIZE];

signal sig1, sig2, sig3, sig4;

int main(int argc, char *argv[]) {
	char* servIP;

	// 신호등 번호 및 gps 값 초기화
	initSignal();

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
	/*
	strcat(buff, (char *)st_sig.id);
	strcat(buff, &st_sig.gSig);
	strcat(buff, buf);
	*/
	//st_sig 구조체 안의 Car 의 raw data를 받았으므로 Car 의 gps 데이터 기반으로 신호 값을 생성해줄 함수
	makeSigSt(&st_sig);

	//st_sig.gSig = (signal *)malloc(sizeof(signal));
	st_sig.gSig.sig_num = 1;
	st_sig.gSig.sig_value = 2;
	{
		char *test = (char *)&st_sig;

	}

	if (sendto(sock_sendrData, (char *)buff, BUFFSIZE, 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) != BUFFSIZE)
		printf("sendto failed");
}

void *send_Request() {
	//var network
	int sock_sendReq;
	struct sockaddr_in servAddr;
	//var data
	char *request = "0";

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

// 신호등 위,경도 및 번호 초기화
void initSignal() {
	sig1.sig_num = 1;
	sig1.gps.latitude = 37.346937;
	sig1.gps.longtitude = 127.736371;

	sig2.sig_num = 2;
	sig2.gps.latitude = 37.346868;
	sig2.gps.longtitude = 126.746831;

	sig3.sig_num = 3;
	sig3.gps.latitude = 37.346865;
	sig3.gps.longtitude = 126.736294;

	sig4.sig_num = 4;
	sig4.gps.latitude = 37.346932;
	sig4.gps.longtitude = 126.736286;
}


// -------------------------- 최장거리 신호요구 mapping 으로 수정!!!! + 신호등 좌우 판별 parameter??
// gps 매핑
int gps_mapping(float c_latitude, float c_longtitude) {
	double distance;						// 차에서 신호등까지의 거리
	float temp;							// 최장거리 도출을 위한 임시 거리 변수
	int sigNum;							// 최종 신호등 번호

										// 신호등 1번 - 차량 간 (거리) 제곱 
	distance = pow((sig1.gps.latitude - c_latitude), 2.0) + pow((sig1.gps.longtitude - c_longtitude), 2.0);

	// 신호등 2번 - 차량 간 (거리) 제곱 
	temp = pow((sig2.gps.latitude - c_latitude), 2.0) + pow((sig2.gps.longtitude - c_longtitude), 2.0);

	if (temp > distance)
		distance = temp;

	// 신호등 3번 - 차량 간 (거리) 제곱 
	temp = pow((sig3.gps.latitude - c_latitude), 2.0) + pow((sig3.gps.longtitude - c_longtitude), 2.0);

	if (temp > distance)
		distance = temp;

	// 신호등 4번 - 차량 간 (거리) 제곱 
	temp = pow((sig4.gps.latitude - c_latitude), 2.0) + pow((sig4.gps.longtitude - c_longtitude), 2.0);

	if (temp > distance)
		distance = temp;

	return distance;
}

// 신호값 도출
void makeSigSt(sigSt *st_sig) {
	st_sig->gSig.sig_num = gps_mapping(st_sig->Car.gps.latitude, st_sig->Car.gps.longtitude);

	// sigValue 값 도출 함수 필요 (작성중)
}

