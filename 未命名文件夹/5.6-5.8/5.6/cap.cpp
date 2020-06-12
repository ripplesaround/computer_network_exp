#define HAVE_REMOTE
#define LINE_LEN 16
#include "winsock.h"
#include <string.h>
#include "pcap.h"

typedef struct ip_address
{ //ip地址 
    u_char b1;
    u_char b2;
    u_char b3;
    u_char b4;
} ip_address;

typedef struct mac_address
{//mac地址 
	u_char b1;
	u_char b2;
	u_char b3;
	u_char b4;
	u_char b5;
	u_char b6;
} mac_address;

typedef struct ethe_header
{ //mac帧首部 
	mac_address mac_dest_address;
	mac_address mac_source_address;
	u_short ether_type;
} ethe_header;

typedef struct ip_header
{ //ip地址首部 
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
} ip_header;

typedef struct udp_header
{ //UPD首部 
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
} udp_header;

typedef struct tcp_header
{ //TCP首部 
	u_short sport;          // 源端口(16位)
    u_short dport;          // 目的端口(16位)
    u_int num;              // 序列号 （32位） 
	u_int ack;              // 确认号（32位） 
	u_short sum;            // 数据偏移（4位），保留（6位），标志位（6位） 
	u_short windonw;        // 窗口 （16位） 
	u_short crc;            // 检验和 （16位） 
	u_short ugr;            // 紧急指针（16位） 
} tcp_header;

void packet_handler(u_char * param, const struct pcap_pkthdr * header, const  u_char *pkt_data);

int ha()
{
    	printf("北京理工大学计算机网络实验--2020.4\n"); 
    	printf("第四章编程实验-WinPcap && C++\n"); 
    	printf("Author：ZHC LX HYF\n");
	pcap_if_t * alldevs, *device;
	int i = 0;
	int iNum;
	u_int netmask;
	struct bpf_program fcode;
	pcap_t * adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	//修改这里可以更改捕获的数据包使用的协议类型 
    char packet_filter[] = "(ip and udp) or (ip and tcp) or (ip and icmp)"; 
    
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{ //获取设备列表 
		fprintf(stderr,"无法打开网络设备:%s\n", errbuf);
		return 1;
	}
	for (device = alldevs; device  != NULL; device = device->next)
	{ //打印列表 
		if (i == 0)
		{
			printf("德以明理 学以精工\n");
			printf("网络设备如下:\n");
		}
		printf("%d. %s\n", ++i, device -> name);
		if (device->description)
			printf(" (%s)\n", device->description);
		else 
			printf("没有设备描述信息!"); 
	}
	if (i == 0)
	{
		printf("\n请先安装WinPcap!");
		return -1;
	}
	printf("请选择网络设备接口:(1 - %d):", i);
	scanf("%d", &iNum);
	if (iNum < 1 || iNum > i)
	{
		printf("设备不存在!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	//跳转到已选设备 
    for (device = alldevs, i = 0;i < iNum -1 ; device = device -> next,i++);
	
    // 打开适配器 
    if ( (adhandle= pcap_open(device->name,  // 设备名
                             65536,     // 要捕捉的数据包的部分 
                                        // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                             PCAP_OPENFLAG_PROMISCUOUS,         // 混杂模式
                             1000,      // 读取超时时间
                             NULL,      // 远程机器验证
                             errbuf     // 错误缓冲池
                             ) ) == NULL)
    {
        fprintf(stderr,"\n不能打开适配器！\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (pcap_datalink(adhandle) != DLT_EN10MB)
	{ //检查数据链路层，为了简单，只考虑以太网
		fprintf(stderr, "\n系统网卡链路出错!\n");
		pcap_freealldevs(alldevs); //释放设备列表 
		return -1;
	}

	if (device->addresses != NULL) //获得接口第一个地址的掩码
		netmask = ((struct sockaddr_in *)(device->addresses->netmask))->sin_addr.S_un.S_addr;
	else //如果接口没有地址，那么我们假设一个C类的掩码
		netmask = 0xffff00;
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{ //编译过滤器
		fprintf(stderr, "不能监听过滤该数据报!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	if (pcap_setfilter(adhandle, &fcode) < 0)
	{ //设置过滤器
		fprintf(stderr, "过滤设置错误!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("正在监听通过%s的数据报...\n", device->description);
	pcap_freealldevs(alldevs); //释放设备列表    
	
	pcap_loop(adhandle, 0, packet_handler, NULL); //开始捕捉
	
	return 0 ;
}

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{ //回调函数，当收到每一个数据包时会被libpcap所调用
	if(header->caplen>400) return;
    struct tm *ltime;
    char timestr[16];
	ip_header * ip_hd;
	udp_header * udp_hd;
	tcp_header * tcp_hd;
    ethe_header * ethe_hd;
	int ip_len,tcp_len,start;
	u_short sport,dport;
	u_short crc,len,check;
	u_int seq,ack;
	printf("\n");
    ltime=localtime(&header->ts.tv_sec); //将时间戳转换为可读字符 
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
    printf("时间：%s\n",timestr);
    
    ethe_hd = (ethe_header *)pkt_data;
    ip_hd = (ip_header *)(pkt_data + 14);
	ip_len = (ip_hd ->ver_ihl & 0xf) * 4; //ip首部长度 
	udp_hd = (udp_header *)((u_char *)ip_hd + ip_len);
	sport = ntohs(udp_hd->sport);
	dport = ntohs(udp_hd->dport);
	crc = ntohs(udp_hd->crc);
	len = ntohs(udp_hd->len);
	int temp1,temp2;
	//tcp_hd = (tcp_header *)((u_char *)ip_hd + ip_len);
///	tcp_len=ntohs(tcp_hd->sum)>>12;
//	check=ntohs(tcp_hd->crc);
//	seq=ntohs(tcp_hd->num);
//	ack=ntohs(tcp_hd->ack);
/*	if(ip_hd->proto==17)
	{
		printf("协议：UDP\n");	
		start=ip_len+8; 
	}
	else if(ip_hd->proto==6)
	{
		printf("协议：TCP\n");
		tcp_hd = (tcp_header *)((u_char *)ip_hd + ip_len);
		tcp_len=ntohs(tcp_hd->sum)>>12;
		start=ip_len+tcp_len*4;
	}
	else if(ip_hd->proto==1)
	{
		printf("协议：ICMP\n");
		start=ip_len+23;
	}
	else printf("协议：其他");
*/	//printf("start=%d\n",start);
if(ip_hd->saddr.b1==(u_char)(0xc0) &&ip_hd->saddr.b2==(u_char)(0xa8)){
	//if(1){
	printf("Before NAT\n");
	 printf("源IP地址: %d.%d.%d.%d      目的IP地址：%d.%d.%d.%d:\n源端口 %d                     目的端口：%d\n源物理地址: %x-%x-%x-%x-%x-%x   目的物理地址：%x-%x-%x-%x-%x-%x\n",
		  ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4, 
		           ip_hd->daddr.b1, ip_hd->daddr.b2, ip_hd->daddr.b3, ip_hd->daddr.b4, sport, dport,
				   ethe_hd->mac_source_address.b1, ethe_hd->mac_source_address.b2, ethe_hd->mac_source_address.b3,
				   ethe_hd->mac_source_address.b4, ethe_hd->mac_source_address.b5, ethe_hd->mac_source_address.b6,
				   ethe_hd->mac_dest_address.b1, ethe_hd->mac_dest_address.b2, ethe_hd->mac_dest_address.b3,
				   ethe_hd->mac_dest_address.b4, ethe_hd->mac_dest_address.b5, ethe_hd->mac_dest_address.b6);
temp1 = ip_hd->saddr.b3;temp2 = ip_hd->saddr.b4;
	ip_hd->saddr.b1=(u_char)(0xd3);
		ip_hd->saddr.b2=(u_char)(0x44);
			ip_hd->saddr.b3=(u_char)(0x09);
				ip_hd->saddr.b4=(u_char)(0x0a);
printf("crc:%d\n",ip_hd->crc);
	printf("flags:%d\n",ip_hd->tlen);
	printf("identification:%d\n",ip_hd->identification);

	
printf("After NAT\n");
printf("NAT: %d.%d.%d.%d -> %d.%d.%d.%d \n",192,168,temp1,temp2,ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4);
	printf("源地址：%x-%x-%x-%x-%x-%x\n",
					ethe_hd->mac_source_address.b1, ethe_hd->mac_source_address.b2, ethe_hd->mac_source_address.b3,
				   ethe_hd->mac_source_address.b4, ethe_hd->mac_source_address.b5, ethe_hd->mac_source_address.b6);
	printf("目标地址：11-11-11-11-11-11\n",
					ethe_hd->mac_dest_address.b1, ethe_hd->mac_dest_address.b2, ethe_hd->mac_dest_address.b3,
				   ethe_hd->mac_dest_address.b4, ethe_hd->mac_dest_address.b5, ethe_hd->mac_dest_address.b6);
/*	
    printf("源IP地址: %d.%d.%d.%d:      目的IP地址：%d.%d.%d.%d:\n源端口：%d                     目的端口：%d\n源物理地址: %x-%x-%x-%x-%x-%x   目的物理地址：%x-%x-%x-%x-%x-%x\n",
		  ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4, 
		           ip_hd->daddr.b1, ip_hd->daddr.b2, ip_hd->daddr.b3, ip_hd->daddr.b4, sport, dport,
				   ethe_hd->mac_source_address.b1, ethe_hd->mac_source_address.b2, ethe_hd->mac_source_address.b3,
				   ethe_hd->mac_source_address.b4, ethe_hd->mac_source_address.b5, ethe_hd->mac_source_address.b6,
				   ethe_hd->mac_dest_address.b1, ethe_hd->mac_dest_address.b2, ethe_hd->mac_dest_address.b3,
				   ethe_hd->mac_dest_address.b4, ethe_hd->mac_dest_address.b5, ethe_hd->mac_dest_address.b6);
*/
	printf("源IP地址: %d.%d.%d.%d      目的IP地址：%d.%d.%d.%d\n",
				ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4, 
		           ip_hd->daddr.b1, ip_hd->daddr.b2, ip_hd->daddr.b3, ip_hd->daddr.b4);
//	printf("IP-lenth:%d\n",len);
//	printf("IP-proto:%d\n",ip_hd->proto);
//	printf("IP-version:4\n",ip_hd->ver_ihl);
//	printf("IP-crc:%d\n",ip_hd->crc);
//	printf("IP-total-lenth:%d\n",ip_hd->tlen);
//	printf("IP-identification:%d\n",ip_hd->identification);
//	printf("IP-flags:%d\n",ip_hd->flags_fo); 
	
	 if(ip_hd->proto==6)
	{
	//	printf("协议：TCP\n");
		tcp_hd = (tcp_header *)((u_char *)ip_hd + ip_len);
		tcp_len=ntohs(tcp_hd->sum)>>12;
		start=ip_len+tcp_len*4;
		check=ntohs(tcp_hd->crc);
		seq=ntohs(tcp_hd->num);
		ack=ntohs(tcp_hd->ack);
		//printf("seq:%x\n",seq);
		//printf("ack:%x\n",ack);
		printf("ack:%d\n",ack);
		printf("seq:%d\n",seq);
		printf("check:%d\n",check);
	}
}
    
}
