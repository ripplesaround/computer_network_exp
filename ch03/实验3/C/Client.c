#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX_SEQ 1
#define inc(k) if (k < MAX_SEQ) k = k + 1; else k = 0
#define FilterError 10
#define FilterLost 10
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

	struct sockaddr_in serverAddr;
	char buffer[1024]="0110000000000010\0";//0x6521=0110010100100001 
	socklen_t addr_size;
    struct timeval timeout = {5, 0};//the timeout is set to 5sec
	
	int frame_id = 1;
	Frame frame_recv;
	Frame frame_send;
	//int frame_expected=0;
	int next_frame_to_send=0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));//a timer for receiver

	memset(&serverAddr,'\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){

        frame_send.seq = next_frame_to_send;
        frame_send.kind = 1;
        
        //printf("Enter Data: ");
        //scanf("%s", buffer);
        frame_send.crc = crc16_ccitt(buffer, 16);
        if((frame_id+3)%FilterError != 0 ){
            //printf("CRC: %x\n", frame_send.crc);
            strcpy(frame_send.packet.data, buffer);
        }else{
            printf("[ERROR]     Frame Error!\n");
            buffer[5] +=1; //Simulation for Frame error
            strcpy(frame_send.packet.data, buffer);
        }

        //Simulation: fliter for loss
        if((frame_id%FilterLost) != 0){
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr_in*)&serverAddr, sizeof(serverAddr));
            //TO DO: start_timer(frame_send)
            printf("[+]Frame Send, next_frame_to_send: %d, frame_send_seq: %d\n",next_frame_to_send, frame_send.seq);
        }else{
            printf("[ERROR]     Frame Lost!\n");
        }


        int addr_size = sizeof(serverAddr);
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr_in*)&serverAddr, &addr_size);
    
        if(f_recv_size > 0 && frame_recv.ack == next_frame_to_send && frame_recv.kind == 0){
            printf("[+]Ack Received, Ack: %d\n", frame_recv.ack);
            inc(next_frame_to_send);
            //TO DO: stop_timer(frame_send)
        }else{
            printf("[+]Ack Not Received, resending now!\n");
        }

        frame_id++;
        sleep(2);
    }

    
}
