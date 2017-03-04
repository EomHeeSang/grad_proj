#ifndef __CARDATA_H
#define __CARDATA_H

#include <stdio.h>
#include <time.h>

#define NORMAL 0
#define RUSH 1
#define EMERGENCY 2

typedef struct carDATA {
	int id;
	int lane_num;
	int curX;
	int curY;
	int speed;
	int distance;
	int emergency;
	//03.03
	//시스템 문서\방향 정의.png 참고하여 gps 기록들을 기반으로 값 설정
	char direction[3];

	time_t cur_time;
	time_t wait_time;
}carData;



#endif
