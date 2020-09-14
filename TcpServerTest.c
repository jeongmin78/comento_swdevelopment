#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX 512
typedef struct Errors {
	char moduleName[30];
	char errorName[30];
	time_t errorTime;
}Errors;

void tcpServer()
{
	int GetSize;
	int len;
	int i;
	int retval;
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	char buf[MAX] = "\0";
	Errors *e;

	int szClntAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("winsock error!\n");
		exit(1);
	}
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		printf("socket() error!\n");
		WSACleanup();
		exit(1);
	}
	printf("socket succeed..\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi("1234"));

	if (bind(hServSock, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind() ¿À·ù\n");
		closesocket(hServSock);
		WSACleanup();
		exit(1);
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		printf("listen() error");
		WSACleanup();
		closesocket(hServSock);
		exit(1);
	}

	printf("listen succeed..\n");

	szClntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)& clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET || hClntSock == SOCKET_ERROR)
	{
		printf("accept() error\n");
		WSACleanup();
		closesocket(hServSock);
		exit(1);
	}

	retval  = recv(hClntSock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		printf("error!\n");
		exit(1);
	}
	else if (retval == 0) {
		exit(1);
	}
	retval = recv(hClntSock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		printf("error!\n");
		exit(1);
	}
	else if (retval == 0) {
		exit(1);
	}
	buf[retval] = '\0';
	e = (Errors*)buf;
	for (i = 0; i < sizeof(e); i++) {
		printf("%s | %s | %lld\n", (e+i)->moduleName, (e+i)->errorName, (e+i)->errorTime);
	}
}

void main()
{
	tcpServer();
}


