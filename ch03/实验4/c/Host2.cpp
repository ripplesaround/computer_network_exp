#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <winsock2.h>
#include <time.h>
#include "CRC1.h"


#pragma comment(lib, "WS2_32.lib")
using namespace std;

int UDPPort = 8888 ;
int Errorrate = 10;
int Lossrate = 10;

int frame_expected = 1;

string Gen = "10001000000100001";

int Correct(string r);

int main(){
		
	WSADATA wsa;
	if( (WSAStartup(MAKEWORD(1,1),&wsa)) != NO_ERROR ){
		printf("Fail to WSADATA startup!");
		return 1;
	}

	SOCKADDR_IN host1, host2;
	SOCKET socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	host2.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    host2.sin_port = htons(UDPPort);
    host2.sin_family = AF_INET;

	int err = bind(socketfd,(SOCKADDR *)&host2,sizeof(host2));
    if (err != 0)
    {
         printf("bind failed:%d\n", WSAGetLastError());
         return 1;
    }

	while(1){
		char recv_buf[400];
		int len = sizeof(SOCKADDR_IN);
		memset(recv_buf,0,sizeof(recv_buf));
		err = recvfrom(socketfd,recv_buf,400,0,(SOCKADDR*)&host1,&len);

		if(err == SOCKET_ERROR){
			printf("Fail to receive from host1");
		}

		string packet = recv_buf;
		int split = packet.find("-");
		string message = packet.substr(0,split);
		string frame_index = packet.substr(split+1,packet.length()-split-1);

		if(atoi(frame_index.c_str()) != frame_expected){
			cout<<"The frame index is not expected!"<<endl;
			continue;
		}

		string crc_code = GetCRC(Gen,message);
		cout<<"frame expected: "<<frame_expected<<endl;
		if(Correct(crc_code)){
			cout<<"CRC check correct! The frame index received is: "<<atoi(frame_index.c_str())<<endl;
			//send back ack
			stringstream sstream;
			string s;
			sstream<<frame_expected;
			sstream>>s;
			sendto(socketfd,s.c_str(),strlen(s.c_str()),0,(SOCKADDR*)&host1,sizeof(SOCKADDR));
			frame_expected++;
		}else{
			cout<<"CRC check incorrect!"<<endl;
		}
		cout<<endl;
		
	}
}

int Correct(string r)
{
    int length = r.length();
    for(int i=0;i<length;i++)
    {
        if(r[i] == '1')
            return 0;
    }
    return 1;
}