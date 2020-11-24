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

	// 初始化套接字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0){
		iRet = WSAGetLastError();
		printf("WSAStartup failed !\n");
		return -1;
	}

	char strSend[BUF_SIZE] = { 0 };
	SOCKADDR_IN servAddr;
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	// 设置服务器地址
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.104");
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(PORT_);

	FILE *fp = NULL;

	while (1){
		char fileAddr[MAXSIZE];
		cout << "需要发送的文件的名称：(@end结束程序)" << endl;
		cin >> fileAddr;
		if (strcmp(fileAddr, "@end") == 0) break;
		if ((fp = fopen(fileAddr, "rb")) == NULL){
			cout << "Fail to fopen!Press any key to exit..." << endl;
			getchar();
			closesocket(sockClient);
			WSACleanup();
			return -1;
		}
		else{
			cout << "Open successfully!" << endl;
		}
		int ret = 0;

		// 向服务器发送数据
		int nServAddLen = sizeof(servAddr);
		while ((ret = fread(strSend, sizeof(char), BUF_SIZE, fp)) != 0){
			if (sendto(sockClient, strSend, BUF_SIZE, 0, (sockaddr *)&servAddr, nServAddLen) == -1){
				cout << "Fail to send to! Press any key to exit..." << endl;
				getchar();
				fclose(fp);
				closesocket(sockClient);
				WSACleanup();
				return -1;
			}
			cout << ret << endl;
		}

		strcpy(strSend, "-@-");

		if (sendto(sockClient, strSend, BUF_SIZE, 0, (sockaddr *)&servAddr, nServAddLen) == -1){
			cout << "Fail to send to! Press any key to exit..." << endl;
			fclose(fp);
			closesocket(sockClient);
			WSACleanup();
			return -1;
		}

	}


	fclose(fp);
	closesocket(sockClient);
	WSACleanup();
	system("pause");
	return 0;
}