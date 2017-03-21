#ifndef __CARDATA_H
#define __CARDATA_H

#include <stdio.h>
#include <time.h>

#define NORMAL 0
#define RUSH 1
#define EMERGENCY 2

typedef struct gpsInfo {
	float latitude, longtitude;							// ���� �浵
}gpsInfo;

typedef struct carDATA {
	int id;
	int lane_num;
	int speed;
	int distance;
	int emergency;
	//03.03
	//++�ý��� ����\���� ����.png �����Ͽ� gps ��ϵ��� ������� �� ����
	gpsInfo gps;
	char direction[3];
	//++���� ��Ÿ� ��� �� ��� �ӵ�
	//++������ �ʿ��� ��ȣ: �յ� ������ ������� ��� ��ȣ������ ��ȯ, 

	time_t cur_time;
	time_t wait_time;
}carData;


#endif
