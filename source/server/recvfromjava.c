#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "../carData.h"
#include "../sigData.h"
#include "/usr/include/mysql/mysql.h"

//network define
#define DATAPORT 5004
#define REQUESTPORT 5005
#define BUFFSIZE 5000
//DB define
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1252"
#define DB_NAME "s_DB"
#define CHOP(x) x[strlen(x) -1] = ' '

//??
#define MAXLANE 10

//DB var
MYSQL *connection = NULL, conn;

//network func
void* recv_rData(void *);
void* recv_req_nSig(void *);
int* measure_confusion(MYSQL_RES *);

//DB func
void insert_rData(char *);
MYSQL_RES* fetchView(int);



int main(){
	//var threads
	pthread_t thID_recvrData;
//	pthread_t thID_recvReq_nSig;

	//DB connect
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);

	if (pthread_create(&thID_recvrData, NULL, recv_rData, NULL) < 0) {
		perror("create error: thread for reciving raw data\n");
		exit(1);
	}
//	if (pthread_create(&thID_recvReq_nSig, NULL, recv_rData, NULL) < 0) {
//		perror("create error: thread for reciving request\n");
//		exit(1);
//	}

	while(1) {
		sleep(10);

	}

	return 0;
}

void* recv_rData(void *p) {
	//var network
	struct sockaddr_in servAddr, clntAddr;
	unsigned int clntLen;
	char buf[BUFFSIZE];
	int sock_recvRdata;
	int clntSock;
	int end;
	int option = 1;

	//create sock
	if ((sock_recvRdata = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket() failed");
	//init servAddr & set servAddr
	//setsockopt(sock_recvRdata, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(DATAPORT);
	if (bind(sock_recvRdata, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		puts("recv data bind failed");

	clntLen = sizeof(clntAddr);
	puts("recv start");
	while (1) {

		if ((end = recvfrom(sock_recvRdata, (void *)buf, BUFFSIZE - 1, 0, (struct sockaddr*)&clntAddr, &clntLen)) < 0)
			printf("recvfrom failed\n");
		buf[end] = '\0';
		puts("recv");
		
		if (buf[0] != '\0') {
			puts("call sql");
			//insert_rData(buf);

			buf[0] = '\0';

		}
		usleep(10);
	}

	pthread_exit(NULL);
}

void* recv_req_nSig(void *p) {
	//var network
	struct sockaddr_in servAddr, clntAddr;
	unsigned int clntLen;
	char buf[BUFFSIZE];
	int sock_recvReqNsig;
	int clntSock;
	int end;
	//var DB
	MYSQL_RES *sql_result;
	int query_stat;
	char query[255];
	//var to be returned
	int *IC_Confusion;
	//create sock
	if ((sock_recvReqNsig = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		puts("socket() failed");
	//init servAddr & set servAddr
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(REQUESTPORT);
	if (bind(sock_recvReqNsig, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		puts("recv req bind failed");

	clntLen = sizeof(clntAddr);
	while (1) {

		if ((end = recvfrom(sock_recvReqNsig, buf, BUFFSIZE - 1, 0, (struct sockaddr*)&clntAddr, &clntLen)) < 0)
			printf("recvfrom failed\n");
		buf[end] = '\0';

		if (buf[0] != '\0') {
			//select from DB
			sprintf(query, "select * from CAR where sig_id ");
			if (mysql_query(connection, query) != 0)
				fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			sql_result = mysql_store_result(connection);
			IC_Confusion = measure_confusion(sql_result);
			//restart

			buf[0] = '\0';
		}
		usleep(10);
	}

	pthread_exit(NULL);
}

int* measure_confusion(MYSQL_RES *sql_res) {
	MYSQL_RES *sql_result = sql_res;
	MYSQL_ROW sql_row;

	int number_cars = 0;
	int avg_speed;
	int demand;


	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
		number_cars++;
	}
	demand = number_cars + avg_speed;
	printf("demand: %d\n", demand);
}

void insert_rData(char *buff) {
	//DB var
	sigSt *sData = (sigSt *)buff;
	signal *sgData = (signal *)(buff + sizeof(int));
	carData *cData = (carData *)(buff + sizeof(int) + sizeof(carData));
	int query_stat;
	char query[255];

	printf("data check\n");
	printf("%d\n", sData->id);
	printf("%d\n", cData->id);
	printf("%d\n", cData->speed);
	printf("%d\n", sgData->sig_num);
	//data from car -> DB
	/*sprintf(query, "INSERT INTO CAR VALUES (NULL, %d, %d, %d, %d, %d, %ld, %d, %d, '%c')",
		cData->id, cData->lane_num, cData->curX, cData->curY, cData->speed, cData->cur_time,
		cData->distance, cData->emergency, cData->direction);
	query_stat = mysql_query(connection, query);
	if (query_stat != 0){
		fprintf(stderr, "Mysql query error : %s\n", mysql_error(&conn));
	}*/
}
