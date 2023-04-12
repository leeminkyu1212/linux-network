#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// 라즈베리파이 IP
char *IP = "192.168.110.179";
char *PORT = "12345";

int sock;

void interrupt(int arg)
{
	printf("\nYou typped Ctrl + C\n");
	printf("Bye\n");

	close(sock);
	exit(1);
}

int main(int argc,char*argv[] )
{
	IP=argv[1];
	PORT=argv[2];
	signal(SIGINT, interrupt);

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

	char buf[100];
	while (1)
	{
		memset(buf, 0, 100);
		scanf("%s", buf);
		if (!strcmp(buf, "exit"))
		{
			write(sock, buf, strlen(buf));
			break;
		}
		write(sock, buf, strlen(buf));
		memset(buf, 0, 100);
		int len = read(sock, buf, 99);
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
