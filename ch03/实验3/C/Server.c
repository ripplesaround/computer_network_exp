#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX_SEQ 1
#define inc(k) if (k < MAX_SEQ) k = k + 1; else k = 0

typedef unsigned short uint16_t;
typedef unsigned char BYTE;

typedef struct packet{
	char data[1024];
}Packet;

typedef struct frame{
	int kind;//ACK:0, SEQ:1 FIN:2
	int seq;
	int ack;
	Packet packet;
	uint16_t crc;
}Frame;

uint16_t crc16_ccitt(BYTE *ucbuf, int iLen){
    
    uint16_t crc = 0xFFFF; // initial value
    uint16_t polynomial = 0x1021; // 0001 0000 0010 0001 (0, 5, 12)

    for (int j = 0; j < iLen; ++j) {
        for (int i = 0; i < 8; i++) {

            char bit = ((ucbuf[j] >> (7-i) & 1) == 1);
            char c15 = ((crc >> 15 & 1) == 1);
            crc <<= 1;
            if (c15 ^ bit) crc ^= polynomial;
        }
    }

    crc &= 0xffff;

    return crc;
}

int main(int argc, char** argv){
	
	if(argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);	
	}
	
	int port = atoi(argv[1]);
	int sockfd;
	struct sockaddr_in serverAddr, newAddr;
	char buffer[1024];
	socklen_t addr_size;

	Frame frame_recv;
	Frame frame_send;
	int frame_expected=0;
	//int next_frame_to_send=0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&serverAddr,'\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(sockfd, (struct sockaddr_in*)&serverAddr, sizeof(serverAddr));
	addr_size = sizeof(newAddr);
	
	while(1){
	 	int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr_in*)&newAddr, &addr_size);
		
		if(f_recv_size>0 && frame_recv.seq == frame_expected && frame_recv.kind == 1){

			int crc = crc16_ccitt(frame_recv.packet.data, 16);
			if(crc != frame_recv.crc){
				printf("[ERROR]		Frame Check Error\n");
				continue;
			}
			printf("[+]CRC Correct, Frame Received: %s\n", frame_recv.packet.data);
			printf("[+]frame_expected: %d\n",frame_expected);
			//TO DO: to_net_work_layer
			inc(frame_expected);
			frame_send.seq = 0;
			frame_send.kind = 0;
			frame_send.ack = 1-frame_expected;
			sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&newAddr, addr_size);
			printf("[+]Ack Send, Ack: %d\n", frame_send.ack);
		}else{
			printf("[+]Frame Not Received\n");
		}
	}

	close(sockfd);
	return 0;

}
