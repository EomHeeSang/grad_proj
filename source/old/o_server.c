#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "/usr/include/mysql/mysql.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1112"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) -1] = ' '

#define defaultTime 10

typedef struct carDATA {
	int id;
	int lane_num;
	int curX;
	int curY;
	int speed;
	time_t time;
	int distance;
	int emergency;
	char direction;
}carData;

//교차로 ID, 신호[0~8], time = 기본 + 추가
typedef struct intersection {
	int ID;
	char curSignal;
	char nextSignal;
	int signaldemand[8];
	time_t signalTime;
}intersection;

int main()
{
	int i, j;
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;

	//input
	carDATA *cData = (carDATA*)malloc(sizeof(carDATA));
	int avgSpeed;
	//output
	intersection *isData = (intersection*)malloc(sizeof(intersection));

	//DB 연결
	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);
	
	//차에서 받은 데이터 DB에 저장
	sprintf(query, "INSERT INTO %s VALUES (NULL, %d, %d, %d, %d, %d, %ld, %d, %d, '%c')",
		"CAR", cData->id, cData->lane_num, cData->curX, cData->curY, cData->speed, cData->time,
		cData->distance, cData->emergency, cData->direction);
	
	//carDB 데이터로 현재 요구치 분석
	//query_stat = mysql_query(connection, "select  from ");
	/*
	int intersectionID;
	int vehicleID;
	int gpsX, gpsY;
	int pastSpeed[10];
	int avgSpeed;
	int direction;
	*/

	//요구치 기록

	//신호제어를 위한 연산
	/*avgSpeed = 0;
	for (i = 0; i < 10; i++) {
		avgSpeed += pastSpeed[i];
	}
	avgSpeed /= 10;

	signalTime = avgSpeed * 10 + defaultTime;
	*/
	//신호 전송

	
	//현재 신호등 상태 DB에 저장
	
	//주기?



    return 0;
}

int send(int signalID, int signalTime) {

}


#include <mysql.h>
#include <string.h>
#include <stdio.h>


#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "gkwlak"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '

int main(void)
{
	MYSQL       *connection = NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char name[12];
	char address[80];
	char tel[12];
	char query[255];

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
		DB_USER, DB_PASS,
		DB_NAME, 3306,
		(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	query_stat = mysql_query(connection, "select * from address");
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	sql_result = mysql_store_result(connection);

	printf("%+11s %-30s %-10s", "이름", "주소", "전화번호");
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%+11s %-30s %-10s", sql_row[0], sql_row[1], sql_row[2]);
	}

	mysql_free_result(sql_result);

	printf("이름 :");
	fgets(name, 12, stdin);
	CHOP(name);

	printf("주소 :");
	fgets(address, 80, stdin);
	CHOP(address);

	printf("전화 :");
	fgets(tel, 12, stdin);
	CHOP(tel);

	sprintf(query, "insert into address values "
		"('%s', '%s', '%s')",
		name, address, tel);

	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	mysql_close(connection);
}