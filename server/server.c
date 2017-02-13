#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "carData.h"
#include "/usr/include/mysql/mysql.h"

//DB define
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1252"
#define DB_NAME "s_DB"
#define CHOP(x) x[strlen(x) -1] = ' '

carData *cData;
char buf[1000];

//DB var
MYSQL *connection = NULL, conn;

void *RepositDatafromCar(void *arg);
void MeasureDemand();

int main(int argc, char *argv[])
{
	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	int servSock;
	pthread_t tid;
	int s_option;
	
	if (argc != 2) {
		/*fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
		exit(1);*/
		ServPort = 4000;
	}
	else
		ServPort = atoi(argv[1]);
	
	//create sock
	if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		puts("socket() failed");
	s_option = 1;
	setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &s_option, sizeof(s_option));

	memset(&ServAddr, 0, sizeof(ServAddr));
	
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(ServPort);
	
	if(bind(servSock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
		puts("bind failed");

	if(listen(servSock, 3) < 0)
		puts("listen failed");

	//DB connect
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);

	//clntLen = sizeof(struct sockaddr_in);

	if (pthread_create(&tid, NULL, RepositDatafromCar, (void *)&servSock) < 0) {
		perror("thread create error");
		exit(1);
	}

	//measure demand
	{
		MYSQL_RES *sql_result;
		MYSQL_ROW sql_row;
		int query_stat;

		int number_cars = 0;
		int avg_speed;
		int demand;

		if (mysql_query(connection, "select CAR_ID from CAR") != 0)
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		sql_result = mysql_store_result(connection);

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			number_cars++;
		}
		demand = number_cars + avg_speed;
	}
	while(1) {
		sleep(10);

	}


	return 0;
}

void *RepositDatafromCar(void *arg) {
	struct sockaddr_in ClntAddr;
	unsigned int clntLen;
	int servSock = *(int *)arg;
	int clntSock = 0;
	int end;

	//DB var
	int query_stat;
	char query[255];

	clntLen = sizeof(ClntAddr);

	while (1) {
		clntSock = accept(servSock, (struct sockaddr *)&ClntAddr, &clntLen);
		printf("accept client %s\n", inet_ntoa(ClntAddr.sin_addr));
		if (clntSock != 0) {
			break;
		}
	}

	while (1) {

		end = recv(clntSock, buf, sizeof(buf), 0);
		buf[end] = '\0';
		cData = (carData *)buf;

		if (buf[0] != '\0') {
			//data from car -> DB
			sprintf(query, "INSERT INTO CAR VALUES (NULL, %d, %d, %d, %d, %d, '%s', %d, %d, '%c')",
				cData->id, cData->lane_num, cData->curX, cData->curY, cData->speed, cData->time,
				cData->distance, cData->emergency, cData->direction);
			query_stat = mysql_query(connection, query);
			if (query_stat != 0)
			{
				fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			}

			printf("id : %d\n", cData->id);
			printf("lane_num : %d\n", cData->lane_num);
			printf("curX : %d\n", cData->curX);
			printf("curY : %d\n", cData->curY);
			printf("speed : %d\n", cData->speed);
			printf("distance : %d\n", cData->distance);
			printf("emergency : %d\n", cData->emergency);
			printf("direction : %c\n", cData->direction);
			printf("time : %s\n", cData->time);
			buf[0] = '\0';

		}
		usleep(10);
	}

	pthread_exit(NULL);
}

//hahaha