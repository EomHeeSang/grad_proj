#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <term.h>
#include <termios.h>
#include "../carData.h"

//network define
#define PORT 5000
#define BUFFSIZE 5000

int getch() {
	int ch;
	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}

int main(int argc, char *argv[])
{
	//var network
	int clntSock;
	struct sockaddr_in servAddr;
	char *servIP;
	//var test
	int chkEnter;

	if (argc != 2) {
		/*fprintf(stderr, "Usage : %s <Server IP> <Port> \n", argv[0]);
		exit(1);*/
		servIP = "192.168.56.1";
	}
	else {
		servIP = argv[1];
	}

	if ((clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		puts("socket Failed\n");

	//servAddr init & set
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(PORT);

	if (connect(clntSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		puts("connection Failed\n");
		return 0;
	}

	char cData[6] = { '1','6','n','n','w','w' };

	while (1) {
		puts("press enter key");
		chkEnter = getch();

		if (chkEnter == 10)
			if (send(clntSock, (char *)cData, sizeof(carData), 0) != sizeof(carData))
				printf("send failed");
		if (chkEnter == 'q') {
			close(clntSock);
			break;
		}

		chkEnter = 0;
	}

	return 0;
}


