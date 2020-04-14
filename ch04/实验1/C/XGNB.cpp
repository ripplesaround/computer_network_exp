#define HAVE_REMOTE
#define LINE_LEN 16
#include "winsock.h"
#include <string.h>
#include "pcap.h"

typedef struct ip_address
{ //ip��ַ 
    u_char b1;
    u_char b2;
    u_char b3;
    u_char b4;
} ip_address;

typedef struct mac_address
{//mac��ַ 
	u_char b1;
	u_char b2;
	u_char b3;
	u_char b4;
	u_char b5;
	u_char b6;
} mac_address;

typedef struct ethe_header
{ //mac֡�ײ� 
	mac_address mac_dest_address;
	mac_address mac_source_address;
	u_short ether_type;
} ethe_header;

typedef struct ip_header
{ //ip��ַ�ײ� 
    u_char  ver_ihl;        // �汾 (4 bits) + �ײ����� (4 bits)
    u_char  tos;            // ��������(Type of service) 
    u_short tlen;           // �ܳ�(Total length) 
    u_short identification; // ��ʶ(Identification)
    u_short flags_fo;       // ��־λ(Flags) (3 bits) + ��ƫ����(Fragment offset) (13 bits)
    u_char  ttl;            // ���ʱ��(Time to live)
    u_char  proto;          // Э��(Protocol)
    u_short crc;            // �ײ�У���(Header checksum)
    ip_address  saddr;      // Դ��ַ(Source address)
    ip_address  daddr;      // Ŀ�ĵ�ַ(Destination address)
    u_int   op_pad;         // ѡ�������(Option + Padding)
} ip_header;

typedef struct udp_header
{ //UPD�ײ� 
    u_short sport;          // Դ�˿�(Source port)
    u_short dport;          // Ŀ�Ķ˿�(Destination port)
    u_short len;            // UDP���ݰ�����(Datagram length)
    u_short crc;            // У���(Checksum)
} udp_header;

typedef struct tcp_header
{ //TCP�ײ� 
	u_short sport;          // Դ�˿�(16λ)
    u_short dport;          // Ŀ�Ķ˿�(16λ)
    u_int num;              // ���к� ��32λ�� 
	u_int ack;              // ȷ�Ϻţ�32λ�� 
	u_short sum;            // ����ƫ�ƣ�4λ����������6λ������־λ��6λ�� 
	u_short windonw;        // ���� ��16λ�� 
	u_short crc;            // ����� ��16λ�� 
	u_short ugr;            // ����ָ�루16λ�� 
} tcp_header;

void packet_handler(u_char * param, const struct pcap_pkthdr * header, const  u_char *pkt_data);

int main()
{
    	printf("��������ѧ���������ʵ��--2020.4\n"); 
    	printf("�����±��ʵ��-WinPcap && C++\n"); 
    	printf("Author��ZHC LX HYF\n");
	pcap_if_t * alldevs, *device;
	int i = 0;
	int iNum;
	u_int netmask;
	struct bpf_program fcode;
	pcap_t * adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	//�޸�������Ը��Ĳ�������ݰ�ʹ�õ�Э������ 
    char packet_filter[] = "(ip and udp) or (ip and tcp) or (ip and icmp)"; 
    
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{ //��ȡ�豸�б� 
		fprintf(stderr,"�޷��������豸:%s\n", errbuf);
		return 1;
	}
	for (device = alldevs; device  != NULL; device = device->next)
	{ //��ӡ�б� 
		if (i == 0)
		{
			printf("�������� ѧ�Ծ���\n");
			printf("�����豸����:\n");
		}
		printf("%d. %s\n", ++i, device -> name);
		if (device->description)
			printf(" (%s)\n", device->description);
		else 
			printf("û���豸������Ϣ!"); 
	}
	if (i == 0)
	{
		printf("\n���Ȱ�װWinPcap!");
		return -1;
	}
	printf("��ѡ�������豸�ӿ�:(1 - %d):", i);
	scanf("%d", &iNum);
	if (iNum < 1 || iNum > i)
	{
		printf("�豸������!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	//��ת����ѡ�豸 
    for (device = alldevs, i = 0;i < iNum -1 ; device = device -> next,i++);
	
    // �������� 
    if ( (adhandle= pcap_open(device->name,  // �豸��
                             65536,     // Ҫ��׽�����ݰ��Ĳ��� 
                                        // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
                             PCAP_OPENFLAG_PROMISCUOUS,         // ����ģʽ
                             1000,      // ��ȡ��ʱʱ��
                             NULL,      // Զ�̻�����֤
                             errbuf     // ���󻺳��
                             ) ) == NULL)
    {
        fprintf(stderr,"\n���ܴ���������\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (pcap_datalink(adhandle) != DLT_EN10MB)
	{ //���������·�㣬Ϊ�˼򵥣�ֻ������̫��
		fprintf(stderr, "\nϵͳ������·����!\n");
		pcap_freealldevs(alldevs); //�ͷ��豸�б� 
		return -1;
	}

	if (device->addresses != NULL) //��ýӿڵ�һ����ַ������
		netmask = ((struct sockaddr_in *)(device->addresses->netmask))->sin_addr.S_un.S_addr;
	else //����ӿ�û�е�ַ����ô���Ǽ���һ��C�������
		netmask = 0xffff00;
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
	{ //���������
		fprintf(stderr, "���ܼ������˸����ݱ�!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	if (pcap_setfilter(adhandle, &fcode) < 0)
	{ //���ù�����
		fprintf(stderr, "�������ô���!\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("���ڼ���ͨ��%s�����ݱ�...\n", device->description);
	pcap_freealldevs(alldevs); //�ͷ��豸�б�    
	
	pcap_loop(adhandle, 0, packet_handler, NULL); //��ʼ��׽
	
	return 0 ;
}

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{ //�ص����������յ�ÿһ�����ݰ�ʱ�ᱻlibpcap������
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
    ltime=localtime(&header->ts.tv_sec); //��ʱ���ת��Ϊ�ɶ��ַ� 
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
    printf("ʱ�䣺%s\n",timestr);
    
    ethe_hd = (ethe_header *)pkt_data;
    ip_hd = (ip_header *)(pkt_data + 14);
	ip_len = (ip_hd ->ver_ihl & 0xf) * 4; //ip�ײ����� 
	udp_hd = (udp_header *)((u_char *)ip_hd + ip_len);
	sport = ntohs(udp_hd->sport);
	dport = ntohs(udp_hd->dport);
	crc = ntohs(udp_hd->crc);
	len = ntohs(udp_hd->len);
	//tcp_hd = (tcp_header *)((u_char *)ip_hd + ip_len);
///	tcp_len=ntohs(tcp_hd->sum)>>12;
//	check=ntohs(tcp_hd->crc);
//	seq=ntohs(tcp_hd->num);
//	ack=ntohs(tcp_hd->ack);
/*	if(ip_hd->proto==17)
	{
		printf("Э�飺UDP\n");	
		start=ip_len+8; 
	}
	else if(ip_hd->proto==6)
	{
		printf("Э�飺TCP\n");
		tcp_hd = (tcp_header *)((u_char *)ip_hd + ip_len);
		tcp_len=ntohs(tcp_hd->sum)>>12;
		start=ip_len+tcp_len*4;
	}
	else if(ip_hd->proto==1)
	{
		printf("Э�飺ICMP\n");
		start=ip_len+23;
	}
	else printf("Э�飺����");
*/	//printf("start=%d\n",start);
	printf("-------------HAHA-------------\n");
	printf("data-link(MAC)-layer��\n");
	printf("���ݱ��ĳ��ȣ�%d\n",header->caplen);
	printf("�ϲ�Э�飺IPv4\n");
	printf("��̫�����ͣ�0800\n"); 
	printf("Դ��ַ��%x-%x-%x-%x-%x-%x\n",
					ethe_hd->mac_source_address.b1, ethe_hd->mac_source_address.b2, ethe_hd->mac_source_address.b3,
				   ethe_hd->mac_source_address.b4, ethe_hd->mac_source_address.b5, ethe_hd->mac_source_address.b6);
	printf("Ŀ���ַ��%x-%x-%x-%x-%x-%x\n",
					ethe_hd->mac_dest_address.b1, ethe_hd->mac_dest_address.b2, ethe_hd->mac_dest_address.b3,
				   ethe_hd->mac_dest_address.b4, ethe_hd->mac_dest_address.b5, ethe_hd->mac_dest_address.b6);
/*	
    printf("ԴIP��ַ: %d.%d.%d.%d:%d      Ŀ��IP��ַ��%d.%d.%d.%d:%d\nԴ�˿ڣ�%d                     Ŀ�Ķ˿ڣ�%d\nԴ�����ַ: %x-%x-%x-%x-%x-%x   Ŀ�������ַ��%x-%x-%x-%x-%x-%x\n",
		  ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4, 
		           ip_hd->daddr.b1, ip_hd->daddr.b2, ip_hd->daddr.b3, ip_hd->daddr.b4, sport, dport,
				   ethe_hd->mac_source_address.b1, ethe_hd->mac_source_address.b2, ethe_hd->mac_source_address.b3,
				   ethe_hd->mac_source_address.b4, ethe_hd->mac_source_address.b5, ethe_hd->mac_source_address.b6,
				   ethe_hd->mac_dest_address.b1, ethe_hd->mac_dest_address.b2, ethe_hd->mac_dest_address.b3,
				   ethe_hd->mac_dest_address.b4, ethe_hd->mac_dest_address.b5, ethe_hd->mac_dest_address.b6);
*/
	printf("---------------------------------------\n");
	printf("network-layer��\n");
	printf("ԴIP��ַ: %d.%d.%d.%d      Ŀ��IP��ַ��%d.%d.%d.%d\n",
				ip_hd->saddr.b1, ip_hd->saddr.b2, ip_hd->saddr.b3, ip_hd->saddr.b4, 
		           ip_hd->daddr.b1, ip_hd->daddr.b2, ip_hd->daddr.b3, ip_hd->daddr.b4);
	printf("IP-lenth:%d\n",len);
	printf("IP-proto:%d\n",ip_hd->proto);
	printf("IP-version:4\n",ip_hd->ver_ihl);
	printf("IP-crc:%d\n",ip_hd->crc);
	printf("IP-total-lenth:%d\n",ip_hd->tlen);
	printf("IP-identification:%d\n",ip_hd->identification);
	printf("IP-flags:%d\n",ip_hd->flags_fo); 
	
	printf("---------------------------------------\n");
	printf("transport-layer��\n");
	if(ip_hd->proto==17)
	{
		printf("Э�飺UDP\n");	
		start=ip_len+8; 
		printf("len:%d\n",len);
		printf("crc:%d\n",crc);
	}
	else if(ip_hd->proto==6)
	{
		printf("Э�飺TCP\n");
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
	else if(ip_hd->proto==1)
	{
		printf("Э�飺ICMP\n");
		start=ip_len+23;
	}
	printf("Դ�˿ڣ�%d\    Ŀ��˿ڣ�%d\n",sport, dport) ;
	printf("---------------------------------------\n");
	printf("application-layer��\n");
	//������ݲ���
    printf("���ݲ�������Ϊ��\n");
    for (int i=start; (i < header->caplen + 1 ) ; i++)
    {
        printf("%.2x ", pkt_data[i-1]);  
        if ( (i % LINE_LEN) == 0) printf("\n");
    }
    printf("\n\n");
  /*  for (int i=start; (i < header->caplen + 1 ) ; i++)
    {
        printf("%c ", pkt_data[i-1]); //Ҳ���Ը�Ϊ %c �� ascii����ʽ����� 
        if ( (i % LINE_LEN) == 0) printf("\n");
    }*/
}
