// time.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include<winsock.h>
#include <conio.h>
#include "time.h"
using namespace std;

//�����µ��׽���֮ǰ��Ҫ����һ������Ws2_32.dll��ĺ���,����������Ϳͻ������Ӳ���
#pragma comment(lib,"ws2_32.lib") 
const char* time_flag = "Time";
int main(int argc, char* argv[])
{
	WSADATA wsaData;                                   //ָ��WinSocket��Ϣ�ṹ��ָ��
	SOCKET sockListener;
	SOCKADDR_IN sin, saClient;                          //����������ַ��sin������//saClient�����ӹ㲥��ַ������Ϣ
	SYSTEMTIME st = { 0 };			
	char cRecvBuff[800];                               //������ջ�����
	char sendBuff[20];
	int nSize, nbSize;
	int iAddrLen = sizeof(saClient);
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)           //����WinSocket�ĳ�ʼ��
	{
		printf("Can't initiates windows socket!Program stop.\n");//��ʼ��ʧ�ܷ���-1
		return -1;
	}
	sockListener = socket(AF_INET, SOCK_DGRAM, 0);
	saClient.sin_family = AF_INET;
	saClient.sin_port = htons(1234);             //���Ͷ�ʹ�õķ��Ͷ˿ڣ����Ը�����Ҫ����
	saClient.sin_addr.s_addr = inet_addr("172.19.244.240");
	//saClient.sin_addr.s_addr = inet_addr("192.168.31.78");
	if (bind(sockListener, (SOCKADDR FAR *)&saClient, sizeof(saClient)) != 0)
	{
		printf("Can't bind socket to local port!Program stop.\n");//��ʼ��ʧ�ܷ���-1
		return -1;
	}
	while (1)
	{
		GetLocalTime(&st);
		nSize = sizeof(SOCKADDR_IN);
		if ((nbSize = recvfrom(sockListener, cRecvBuff, 800, 0,(SOCKADDR FAR *) &sin, &nSize)) == SOCKET_ERROR) //������ʧ������ʾ����
		{
			printf("Recive Error");
			break;
		}
		cRecvBuff[nbSize] = '\0';                              //�ַ�����ֹ
		if ((NULL != strstr((const char *)cRecvBuff, (const char *)time_flag)))
		{
			static long count = 0;
			//printf("%s\n", cRecvBuff);                              //��ʾ�����յ����ַ���
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