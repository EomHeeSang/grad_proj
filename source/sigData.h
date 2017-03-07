#ifndef __SIGDATA_H
#define __SIGDATA_H

#include <stdio.h>
#include "carData.h"

#define NORMAL 0
#define RUSH 1
#define EMERGENCY 2

// ��ȣ��
typedef struct signal {
	int sig_num;			// ��ȣ�� ��ȣ
	int sig_value;			// ��ȣ�� (����, ��ȸ��)
}signal;

// �������� ��ȣ �䱸 + ��Ʈ�ѷ� id. ������ ����
typedef struct sigSt {
	int id;					// ��Ʈ�ѷ��� ��ġ�� ������ ���� id
	carData *Car;			// ������ raw ������
	signal *gSig;			// ���� gps���� ���� Ȯ�ε� �䱸 ��ȣ�� ��ȣ
	//++���� ������ ���������� �Ÿ�
}sigSt;

// ���� �� �������� ��ȣ ���� ����
typedef struct sigInfo {
	int c_sig;				// ���� ��ȣ
	int c_stime;			// ���� ��ȣ�� ���� �ð�
	int n_sig;				// ���� ��ȣ
	int n_stime;			// ���� ��ȣ�� ���� �ð�
}sigInfo;


// ��ȣ �䱸ġ. �������� ����
typedef struct sigDemand {
	int emergency;			// ���޻�Ȳ���� (�켱����)
	signal Sig;				// ��ȣ ��ȣ + ��ȣ �� 
	int keep_time;			// ��ȣ ���� �ð�
	struct signal *next;	// ���� ��ȣ(���) �ּ�
}sigDemand;

// ��ȣ ��⿭(ť)
typedef struct sigQueue {
	int count;				// ť ����
	signal front;			// ���� ��ȣ���� �ּ�
}sigQueue;

#endif
