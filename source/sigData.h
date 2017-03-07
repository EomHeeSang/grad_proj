#ifndef __SIGDATA_H
#define __SIGDATA_H

#include <stdio.h>
#include "carData.h"

#define NORMAL 0
#define RUSH 1
#define EMERGENCY 2

// 신호등
typedef struct signal {
	int sig_num;			// 신호등 번호
	int sig_value;			// 신호값 (직진, 좌회전)
}signal;

// 차량들의 신호 요구 + 컨트롤러 id. 서버로 전송
typedef struct sigSt {
	int id;					// 컨트롤러가 위치한 교차로 고유 id
	carData *Car;			// 차량의 raw 데이터
	signal *gSig;			// 차량 gps값을 통해 확인된 요구 신호등 번호
	//++현재 차량과 정지선과의 거리
}sigSt;

// 차량 및 서버에게 신호 정보 전달
typedef struct sigInfo {
	int c_sig;				// 현재 신호
	int c_stime;			// 현재 신호의 남은 시간
	int n_sig;				// 다음 신호
	int n_stime;			// 다음 신호의 유지 시간
}sigInfo;


// 신호 요구치. 서버에서 수신
typedef struct sigDemand {
	int emergency;			// 응급상황정도 (우선순위)
	signal Sig;				// 신호 번호 + 신호 값 
	int keep_time;			// 신호 유지 시간
	struct signal *next;	// 다음 신호(노드) 주소
}sigDemand;

// 신호 대기열(큐)
typedef struct sigQueue {
	int count;				// 큐 갯수
	signal front;			// 이전 신호정보 주소
}sigQueue;

#endif
