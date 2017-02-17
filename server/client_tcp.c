#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <term.h>
#include <termios.h>
#include "carData.h"

int getch() {
        int ch;
        struct termios buf;
        struct termios save;

        tcgetattr(0, &save);
        buf = save;
        buf.c_lflag &= ~(ICANON|ECHO);
        buf.c_cc[VMIN] = 1;
        buf.c_cc[VTIME] = 0;
        tcsetattr(0, TCSAFLUSH, &buf);
        ch = getchar();
        tcsetattr(0, TCSAFLUSH, &save);

        return ch;
}

int main(int argc, char *argv[])
{
	int sock;	
	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	char *servIP;
	int buf_size;
	time_t tim;
	char *t;
	int chr;

	if (argc != 3) {
		/*fprintf(stderr, "Usage : %s <Server IP> <Port> \n", argv[0]);
		exit(1);*/
		servIP = "127.0.0.1";
		ServPort = 4000;
	}
	else {
		servIP = argv[1];
		ServPort = atoi(argv[2]);
	}

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		puts("socket Failed\n");

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(servIP);
	ServAddr.sin_port = htons(ServPort);

	if (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0) {
		puts("connect Failed\n");
		return 0;
	}

	time(&tim);

	carData cData;
	
	cData.id = 339;
	cData.lane_num = 2;
	cData.curX = 100;
	cData.curY = 200;
	cData.speed = 80;
	cData.distance = 20;
	cData.emergency = EMERGENCY;
	cData.direction = 'l';

	t = malloc(sizeof(char)*100);
	strcpy(t, ctime(&tim));
	sprintf(cData.time, "%s", t);

	buf_size = sizeof(int)*20 + 1;
	
	printf("time : %s\n", cData.time);

	while(1) {
		puts("press enter key");
		chr = getch();

		if(chr == 10)
		send(sock, (char *)&cData, buf_size,0);
		
		chr = 0;
	}

	return 0;
}


