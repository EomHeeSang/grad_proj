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
	char direction;
	time_t time;
}carData;

#endif
