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

int windowSize = 4;
int already_send = 0;
int ack_expected = 1;
int waitCount = 0;
int next_frame_to_send = 1;

int UDPPort = 8888 ;
int Errorrate = 10;
int Lossrate = 10;


char buffer[1000];
string Gen = "10001000000100001";

string message[50] = {};
int message_index = 0;

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
	
	ifstream testdata("test.txt");
	while(!testdata.eof()){
		string line;
		getline(testdata,line);
		message[message_index]=line;
		message_index++;
	}

	while(1){

		if(already_send<windowSize && next_frame_to_send<=message_index){
			//CRC
			string m = message[next_frame_to_send-1];
			string crc_code = GetCRC(Gen, m);

			stringstream sstream;
			sstream << next_frame_to_send;
			string next_frame_index;
			sstream >> next_frame_index;

			string packet = m + crc_code + "-" + next_frame_index; //- is used as a split

			cout<<"ack_expected: "<<ack_expected<<endl;
			cout<<"next_frame_to_send: "<<next_frame_to_send<<endl;

			int rd = rand()%100;
			//Simulation for transmission error
			if(rd<=Errorrate){
				cout<<"[++++]Frame data error during transmission!"<<endl;
				packet.replace(0,2,"00");//error
				sendto(socketfd, packet.c_str(), packet.length(), 0, (SOCKADDR *)&host2, sizeof(SOCKADDR));
				next_frame_to_send++;
			}
			//simulation for transmission loss
			else if(rd>=100-Lossrate){
				cout<<"[++++]Frame data loss during transmission!"<<endl;
				next_frame_to_send++;
			}
			else{
				next_frame_to_send++;
				sendto(socketfd, packet.c_str(), packet.length(), 0, (SOCKADDR *)&host2, sizeof(SOCKADDR));
			}

			already_send++;
		}

		int len, nResult;
        len = sizeof(SOCKADDR_IN);
        char buf[400];
        memset(buf, -1, sizeof(buf));
        unsigned long on = 1;
    	if (0 != ioctlsocket(socketfd, FIONBIO, &on)) {cout<<"Failed !!"<<endl;}
        nResult = recvfrom(socketfd, buf, 400, 0, (SOCKADDR *)&host2, &len);
		if (nResult < 0) {
			waitCount++;
			//20 次等待 ack 则超时重传
			if (waitCount > 20){
				//超时重置
				printf("Time out and send again!\n");
				next_frame_to_send = ack_expected;
				already_send = 0;
				waitCount = 0;
			}
			else {
				waitCount ++;
			}
			Sleep(500);
        }

        if(atoi(buf) == ack_expected){
			ack_expected++;
        }

        if(ack_expected == message_index){
            cout<< "Transport finished successfully!" << endl;
            break;
		}
	}
}