#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define NAME_SIZE 20
#define MSG_SIZE 100

char *IP = "192.168.110.231";
char *PORT = "12345";

// pthread_t send_tid;
// pthread_t receive_tid;
int exitFlag;
int sock;

// 채팅창에 보여질 이름의 형태
char name[NAME_SIZE] = "[DEFAULT]";
char msg[MSG_SIZE];

void interrupt(int arg)
{
	printf("\nYou typped Ctrl + C\n");
	printf("Bye\n");

	// pthread_cancel(send_tid);
	// pthread_cancel(receive_tid);
	// pthread_join(send_tid, 0);
	// pthread_join(receive_tid, 0);

	close(sock);
	exit(1);
}

// 커멘드 아규먼트 필요로 함: 사용할 닉네임
int main(int argc, char *argv[])
{
	signal(SIGINT, interrupt);

	// sprintf 사용해서, [닉네임] 형태로 name 저장
	sprintf(name, "[%s]", argv[1]);

	IP = argv[2];
	PORT = argv[3];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("ERROR :: 1_Socket Create Error\n");
		exit(1);
	}

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(atoi(PORT));

	// Connect
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("ERROR :: 2_Connect Error\n");
		exit(1);
	}

	char buf[NAME_SIZE + MSG_SIZE];
	while (1)
	{

		memset(buf, 0, NAME_SIZE + MSG_SIZE);
		scanf(msg, MSG_SIZE, stdin);
		if (!strcmp(msg, "exit"))
		{
			write(sock, msg, strlen(msg));
			break;
		}
		sprintf(buf, "%s", msg);
		write(sock, buf, strlen(buf));
		
		////
		memset(buf, 0, 100);
		
		int len = read(sock, buf, NAME_SIZE + MSG_SIZE - 1);
		if (len == 0)
		{
			printf("INFO :: Server Disconnected\n");
			break;
		}
		printf("%s\n", buf);

	}

	// close sock
	close(sock);
	return 0;
}
