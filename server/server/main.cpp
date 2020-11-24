#include <stdio.h>
#include <winsock2.h>
#include<iostream>
#define MAXSIZE 255
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#define	BUF_SIZE	1024
#define PORT_		9411

int main(void)
{
	WSADATA wsd;
	int iRet = 0;

	// ��ʼ���׽��ֶ�̬��
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0){
		printf("WSAStartup failed:%d!\n", WSAGetLastError());
		return -1;
	}

	SOCKET socketSrv = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrClient;
	char strRecv[BUF_SIZE] = { 0 }, strSend[BUF_SIZE] = "udp server send";
	int len = sizeof(SOCKADDR);

	// ���÷�������ַ
	ZeroMemory(strRecv, BUF_SIZE);
	//addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT_);

	// ���׽���
	iRet = bind(socketSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == iRet)
	{
		printf("bind failed%d!\n", WSAGetLastError());
		closesocket(socketSrv);
		WSACleanup();
	}

	FILE *fp = NULL;
	char fileAddr[MAXSIZE] = { 0 };
	int ret = 0;
	if ((fp = fopen(fileAddr, "wb")) == NULL){ 
		cout << "Failed to open! Press any key to exit..." << endl;
		getchar();
		closesocket(socketSrv);
		WSACleanup();
		return -1;
	}

	// �ӿͻ��˽�������
	printf("udp server start...\n");
	while (TRUE)
	{
		while ((ret=recvfrom(socketSrv, strRecv, BUF_SIZE, 0, (SOCKADDR*)&addrClient, &len)) > 0){
			if (strcmp(strRecv, "-@-") == 0){
				break;
			}
			fwrite(strRecv, ret, 1, fp);
			fflush(fp);
			cout << ret << endl;
		}
		printf("Recv From Client\n");
	}
	fclose(fp);
	closesocket(socketSrv);
	WSACleanup();

	return 0;
}