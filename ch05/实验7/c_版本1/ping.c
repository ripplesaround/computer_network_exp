#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SZ 1024
#define PACK_LEN 32

int sendnum = 0;  //发送包编号
int recvnum = 0;
char sendpack[BUF_SZ];    //发送缓冲区
char recvpack[BUF_SZ];
long total_time = 0;
int max_time = 0;
int min_time = 0x3ffffff;

unsigned short chksum(unsigned short* addr, int len)   //校验和
{
  unsigned int ret = 0;

  while(len > 1)
  {
    ret += *addr++;
    len -= 2;
  }
  if(len == 1)
  {
    ret += *(unsigned char*)addr;
  }

  ret = (ret >> 16) + (ret & 0xffff);
  ret += (ret >> 16);

  return ~ret;
}

void send_packet(int icmp_socket, struct sockaddr_in s)
{
  sendnum++;
  memset(sendpack, 0x00, sizeof(sendpack));
  struct icmp* p= (struct icmp*)sendpack;
  p->icmp_type  = ICMP_ECHO;
  p->icmp_code  = 0;
  p->icmp_cksum = 0;
  p->icmp_seq   = sendnum;
  p->icmp_id    = getpid();
  struct timeval tval;
  gettimeofday(&tval, NULL);
  memcpy((void*)p->icmp_data, (void*)&tval, sizeof(tval));
  p->icmp_cksum = chksum((unsigned short*)sendpack, PACK_LEN + 8);
  sendto(icmp_socket,sendpack, PACK_LEN + 8, 0, (struct sockaddr*)&s, sizeof(s));
}

void recv_packet(int icmp_socket)
{
  struct sockaddr_in from;
  memset(recvpack, 0x00, sizeof(recvpack));
  socklen_t len = sizeof(from);
  recvnum++;
  if(recvfrom(icmp_socket, recvpack, BUF_SZ, 0, (struct sockaddr*)&from, &len) < 0)
  {
    printf("request timeout\n");
    recvnum--;
    return;
  }
  struct ip* pip = (struct ip*)recvpack;
  struct icmp* picmp = (struct icmp*)(recvpack+ (pip->ip_hl << 2));
  struct timeval end;
  gettimeofday(&end, NULL);
    
  long dif = ((&end)->tv_sec - ((struct timeval*)picmp->icmp_data)->tv_sec)* 1000 + ((&end)->tv_usec - ((struct timeval*)picmp->icmp_data)->tv_usec) / 1000;
  printf("from %s: size=%d ttl=%d time=%ld ms\n", inet_ntoa(from.sin_addr), PACK_LEN, pip->ip_ttl, dif);
  total_time += dif;
  if(dif < min_time) min_time = dif;
  if(dif > max_time) max_time = dif;
}


int main(int argc, char* argv[]) {
  in_addr_t addr;
  struct sockaddr_in s;

  if(argc != 2)
  {
    printf("arguments error\n");
    return 1;
  }
  s.sin_family = AF_INET;

  if(inet_addr(argv[1]) == -1)
  {
    struct hostent* pent = gethostbyname(argv[1]);
    if(pent != NULL)
    {
      memcpy((char*)&s.sin_addr, (char*)pent->h_addr, pent->h_length);
    }
    else
    {
      printf("gethostbyname error\n");
      return -1;
    }
  }
  else
  {
    s.sin_addr.s_addr = inet_addr(argv[1]);
  }

  int icmp_socket;
  if((icmp_socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
  {
    printf("socket create error\n");
    return -1;
  }
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  if (setsockopt(icmp_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) 
  {
    printf("socket set error\n");
    return -1;
  }
  int i = 0;
  printf("PING %s (%s) %d bytes of data.\n", argv[1], inet_ntoa(s.sin_addr), PACK_LEN);
  for(i = 0; i < 5; i++)
  {
    send_packet(icmp_socket,s);
    recv_packet(icmp_socket);
    sleep(1);
  }
  printf("from %s ping statistics: \n",inet_ntoa(s.sin_addr));
  printf("\t%d packets sent, %d received, %.2f%% packet loss\n", sendnum, recvnum, ((double)(sendnum - recvnum) / sendnum) * 100);
  if(max_time != 0)
  {
    printf("Round trip time estimation: \n");
    printf("\tmin: %d, max: %d, average: %d\n",min_time, max_time, (int)(total_time/i));
  }

  return 0;
}
