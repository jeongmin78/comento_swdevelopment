#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <io.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma warning(disable : 4996)
#define MAX 512

struct _finddata_t td;

struct DelayModule {
	char moduleName[30];
	char errorName[30];
	time_t errorTime;
};
struct ErrorModule {
	char moduleName[30];
	char errorName[30];
	time_t errorTime;
};
struct Errors {
	char moduleName[30];
	char errorName[30];
	time_t errorTime;
};
struct DelayModule dmList[MAX];
struct ErrorModule emList[MAX];
int count1;
int count2;
void addDelayModule(struct DelayModule* dm);
void addErrorModule(struct ErrorModule* em);
void print_day(struct tm* t); 

void checkModuleUpdate() {
	struct DelayModule dm;
	time_t now = time(NULL);
	
	struct _finddata_t fd;
	long handle;
	int result = 1;
	handle = _findfirst(".\\*.txt", &fd);
	if (handle == -1)
	{
		printf("There were no files.\n");
		return;
	}

	while (result != -1)
	{
		//printf("File: %s\n", fd.name);
		//printf("File: %d\n", fd.time_access);
		//printf("t: %d\n", now-fd.time_write);
		
		if (now - fd.time_write > 3) {
			strcpy(dm.moduleName, fd.name);
			dm.errorTime = fd.time_write;
			addDelayModule(&dm);
		}
		result = _findnext(handle, &fd);
	}

	_findclose(handle);
}
void checkModuleError() {
	struct ErrorModule em;
	int lineCount;
	char buffer[MAX];
	FILE* pfile = NULL;
	struct _finddata_t fd;
	long handle;
	int result = 1;
	handle = _findfirst(".\\*.txt", &fd);
	while (result != -1)
	{
		if (0 == fopen_s(&pfile , fd.name, "rt")) {//파일 오픈 성공
			lineCount = 1;
			while (fgets(buffer, 30, pfile) != NULL) { //개행제거
				if (buffer[strlen(buffer) - 1] == '\n') {
					buffer[strlen(buffer) - 1] = '\0';
				}
				if (lineCount > 1) {
					strcpy(em.moduleName, fd.name);
					strcpy(em.errorName, buffer);
					em.errorTime = fd.time_write;
					addErrorModule(&em);
					//printf("%d, %s\n", lineCount, buffer);
					//printf("name: %s\nerrorname : %s\ntime: %lld\n\n", fd.name, buffer, fd.time_write);
				}
				lineCount++;
			}
			fclose(pfile);
		}
		result = _findnext(handle, &fd);
	}
	_findclose(handle);
}
void addDelayModule(struct DelayModule *dm) {
	strcpy(dmList[count1].moduleName, dm->moduleName);
	dmList[count1].errorTime = dm->errorTime;
	//printf("%s, %d\n",dmList[count].moduleName, dmList[count].errorTime);
	count1++;
}
void addErrorModule(struct ErrorModule *em) {
	strcpy(emList[count2].moduleName, em->moduleName);
	strcpy(emList[count2].moduleName, em->errorName);
	emList[count2].errorTime = em->errorTime;
	printf("%s, %s, %lld\n",emList[count2].moduleName, emList[count2].errorName, emList[count2].errorTime);
	count2++;
}


//void sendErrors(struct Errors e) {
void tcpClient(char *IPaddress, char *portNumber)
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char buf[MAX]="\0";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("winsock error!\n");
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		printf("socket() error!\n");
		WSACleanup();
		exit(1);
	}
	printf("socket succeed..\n");

	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IPaddress); // CnC Server IP address 
	servAddr.sin_port = htons(atoi(portNumber)); // CnC Server Port Number

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("connect() error!\n");
		WSACleanup();
		closesocket(hSocket);
		exit(1);
	}
	printf("connect succeed..\n");

	printf("메시지를 입력하세요.\n");
	gets_s(buf, MAX);
	send(hSocket, buf, MAX, 0);
}



void removeErrors(struct Errors e) {

}
void mergeErrors(struct DelayModule dm, struct ErrorModule em) {
}


void main() 
{
	int i;
	//tcpClient("127.0.0.1","1234");
	//checkModuleUpdate();
	checkModuleError();
	//for(i=0; i<count1; i++){
	//	printf("dm : %s, %s\n", (dmList+i)->moduleName, (dmList+i)->errorName); //print_day(dmList->errorTime);
	//	//printf("em : %s, %s", emList->moduleName, emList->errorName); //print_day(emList->errorTime);

	//}
}

void print_day(struct tm* t) {
	printf("%d/%d/%d %d:%d:%d\n", 1900 + t->tm_year, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}