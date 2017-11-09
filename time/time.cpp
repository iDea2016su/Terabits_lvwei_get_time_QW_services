// time.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include<winsock.h>
#include <conio.h>
#include "time.h"
using namespace std;

//创建新的套接字之前需要调用一个引入Ws2_32.dll库的函数,否则服务器和客户端连接不上
#pragma comment(lib,"ws2_32.lib") 
const char* time_flag = "Time";
int main(int argc, char* argv[])
{
	WSADATA wsaData;                                   //指向WinSocket信息结构的指针
	SOCKET sockListener;
	SOCKADDR_IN sin, saClient;                          //设置两个地址，sin用来绑定//saClient用来从广播地址接收消息
	SYSTEMTIME st = { 0 };			
	char cRecvBuff[800];                               //定义接收缓冲区
	char sendBuff[20];
	int nSize, nbSize;
	int iAddrLen = sizeof(saClient);
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)           //进行WinSocket的初始化
	{
		printf("Can't initiates windows socket!Program stop.\n");//初始化失败返回-1
		return -1;
	}
	sockListener = socket(AF_INET, SOCK_DGRAM, 0);
	saClient.sin_family = AF_INET;
	saClient.sin_port = htons(1234);             //发送端使用的发送端口，可以根据需要更改
	saClient.sin_addr.s_addr = inet_addr("172.19.244.240");
	//saClient.sin_addr.s_addr = inet_addr("192.168.31.78");
	if (bind(sockListener, (SOCKADDR FAR *)&saClient, sizeof(saClient)) != 0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//初始化失败返回-1
		return -1;
	}
	while (1)
	{
		GetLocalTime(&st);
		nSize = sizeof(SOCKADDR_IN);
		if ((nbSize = recvfrom(sockListener, cRecvBuff, 800, 0,(SOCKADDR FAR *) &sin, &nSize)) == SOCKET_ERROR) //若接收失败则提示错误
		{
			printf("Recive Error");
			break;
		}
		cRecvBuff[nbSize] = '\0';                              //字符串终止
		if ((NULL != strstr((const char *)cRecvBuff, (const char *)time_flag)))
		{
			static long count = 0;
			//printf("%s\n", cRecvBuff);                              //显示所接收到的字符串
			sendBuff[0] = 'T';
			sendBuff[1] = 'i';
			sendBuff[2] = 'm';
			sendBuff[3] = 'e';
			sendBuff[4] = (int(st.wYear)/10)%10+'0';
			sendBuff[5] = (int(st.wYear)%10) + '0';
			sendBuff[6] = (int(st.wMonth) / 10) % 10 + '0';
			sendBuff[7] = (int(st.wMonth) % 10) + '0';
			sendBuff[8] = (int(st.wDay) / 10) % 10 + '0';
			sendBuff[9] = (int(st.wDay) % 10) + '0';
			sendBuff[10] = (int(st.wHour) / 10) % 10 + '0';
			sendBuff[11] = (int(st.wHour) % 10) + '0';
			sendBuff[12] = (int(st.wMinute) / 10) % 10 + '0';
			sendBuff[13] = (int(st.wMinute) % 10) + '0';
			sendBuff[14] = (int(st.wSecond) / 10) % 10 + '0';
			sendBuff[15] = (int(st.wSecond) % 10) + '0';
			sendBuff[16] = 'T';
			sendBuff[17] = 'D';
			sendBuff[18] = '\0';
			sendto(sockListener, sendBuff, sizeof(sendBuff), 0, (SOCKADDR*)&sin, nSize);
			count++;
			printf("Package Number:%d\r\n",count);
		}
	}
	return 0;
}