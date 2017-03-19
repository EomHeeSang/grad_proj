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

int main(int argc, char *argv[]){
	char* servIP;

	// ��ȣ�� ��ȣ �� gps �� �ʱ�ȭ
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
	//sig_num �Ǻ� �Լ�

	st_sig.gSig.sig_num = 1;
	st_sig.gSig.sig_value = 2;

	strcat(buff, *(char *)st_sig.id);
	strcat(buff, *(char *)st_sig.gSig);
	strcat(buff, buf);

	//st_sig ����ü ���� Car �� raw data�� �޾����Ƿ� Car �� gps ������ ������� ��ȣ ���� �������� �Լ�
	makeSigSt(&st_sig);

	//st_sig.gSig = (signal *)malloc(sizeof(signal));
	//st_sig.gSig->sig_num = 1;
	//st_sig.gSig->sig_value = 2;
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

// ��ȣ�� ��,�浵 �� ��ȣ �ʱ�ȭ
void initSingal(signal *signal) {
	sig1.sig_num = 1;
	sig1.latitude = 37.346937;
	sig1.longtitude = 127.736371;

	sig2.sig_num = 2;
	sig2.latitude = 37.346868;
	sig2.longtitude = 126.746831;

	sig3.sig_num = 3;
	sig3.latitude = 37.346865;
	sig3.longtitude = 126.736294;

	sig4.sig_num = 4;
	sig4.latitude = 37.346932;
	sig4.longtitude = 126.736286;
}

float GetAngleBetweenTwoVectors(float Vec1X, float Vec1Y, float Vec2X, float Vec2Y) {
	float v;

	// ���� �� ���͸� ���� ���ͷ� �����.
	v = sqrt(Vec1X*Vec1X + Vec1Y*Vec1Y);
	Vec1X /= v;
	Vec1Y /= v;

	v = sqrt(Vec2X*Vec2X + Vec2Y*Vec2Y);
	Vec2X /= v;
	Vec2Y /= v;

	// �����ϱ�
	float theta;
	float degree;

	theta = Vec1X*Vec2X + Vec2Y*Vec2Y;

	theta = acos(theta);

	degree = theta * (180 / 3.141592);

	return degree;
}

// gps ����
int gps_mapping(float c_latitude, float c_longtitude) {
	float CtoI_lat;						// ������ ������ �߾ӱ����� ��������
	float CtoI_long;					// ������ ������ �߾ӱ����� �浵����
	float ItoS_lat;						// ������ �߾ӿ��� ��ȣ������� ��������
	float ItoS_long						// ������ �߾ӿ��� ��ȣ������� �浵����
	float i_latitude, i_longtitude;		// ������ �߾��� ����, �浵
	float degree;
	int sigNum;							// ���� ��ȣ�� ��ȣ

										// ������ �߾��� ����, �浵 ��
	i_latitude = (sig1.latitude + sig2.latitude + sig3.latitude + sig4.latitude) / 4;
	i_longtitude = (sig1.longtitude + sig2.longtitude + sig3.longtitude + sig4.longtitude) / 4;

	// ������ ������ �߾ӱ��� ����, �浵 ����
	CtoI_lat = i_latitude - c_latitude;
	CtoI_long = i_longtitude - c_longtitude;

	// ������ �߾ӿ��� ��ȣ��1 ������ ����, �浵����
	ItoS_lat = sig1.latitude - i_latitude;
	ItoS_long = sig1.longtitude - i_longtitude;

	// ���� �� ���� (������� - ����ü �迭�� ���� ����)
	degree = GetAngleBetweenTwoVectors(CtoI_lat, CtoI_long, ItoS_lat, ItoS_long);

	if (degree >= 0 || degree <= 30) {
		sigNum = 1;
		return sigNum;
	}

	// ������ �߾ӿ��� ��ȣ��2 ������ ����, �浵����
	ItoS_lat = sig2.latitude - i_latitude;
	ItoS_long = sig2.longtitude - i_longtitude;

	degree = GetAngleBetweenTwoVectors(CtoI_lat, CtoI_long, ItoS_lat, ItoS_long);

	if (degree >= 0 || degree <= 30) {
		sigNum = 2;
		return sigNum;
	}

	// ������ �߾ӿ��� ��ȣ��2 ������ ����, �浵����
	ItoS_lat = sig3.latitude - i_latitude;
	ItoS_long = sig3.longtitude - i_longtitude;

	degree = GetAngleBetweenTwoVectors(CtoI_lat, CtoI_long, ItoS_lat, ItoS_long);

	if (degree >= 0 || degree <= 30) {
		sigNum = 3;
		return sigNum;
	}

	// ������ �߾ӿ��� ��ȣ��2 ������ ����, �浵����
	ItoS_lat = sig4.latitude - i_latitude;
	ItoS_long = sig4.longtitude - i_longtitude;

	degree = GetAngleBetweenTwoVectors(CtoI_lat, CtoI_long, ItoS_lat, ItoS_long);

	if (degree >= 0 || degree <= 30) {
		sigNum = 4;
		return sigNum;
	}
}

// ��ȣ�� ����
void makeSigSt(sigSt *st_sig) {
	st_sig->gSig.sig_num = gps_mapping(st_sig->Car.gps.latitude, st_sig->Car.gps.longtitude);

	// sigValue �� ���� �Լ� �ʿ� (�ۼ���)
}