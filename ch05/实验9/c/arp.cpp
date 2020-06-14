#include "stdio.h"
#include "pcap.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#define ETH_ARP         0x0806  //��̫��֡���ͱ�ʾ�������ݵ����ͣ�����ARP�����Ӧ����˵�����ֶε�ֵΪx0806
#define ARP_HARDWARE    1  //Ӳ�������ֶ�ֵΪ��ʾ��̫����ַ
#define ETH_IP          0x0800  //Э�������ֶα�ʾҪӳ���Э���ַ����ֵΪx0800��ʾIP��ַ
#define ARP_REQUEST     1   //ARP����
#define ARP_RESPONSE       2      //ARPӦ��


struct EthernetHeader
{
    u_char DestMAC[6];    //Ŀ��MAC��ַ 6�ֽ�
    u_char SourMAC[6];   //ԴMAC��ַ 6�ֽ�
    u_short EthType;         //��һ��Э�����ͣ���0x0800������һ����IPЭ�飬0x0806Ϊarp  2�ֽ�
};

struct ArpHeader
{
    unsigned short hdtype;   //Ӳ������
    unsigned short protype;   //Э������
    unsigned char hdsize;   //Ӳ����ַ����
    unsigned char prosize;   //Э���ַ����
    unsigned short op;   //�������ͣ�ARP����1����ARPӦ��2��
    u_char smac[6];   //ԴMAC��ַ
    u_char sip[4];   //ԴIP��ַ
    u_char dmac[6];   //Ŀ��MAC��ַ
    u_char dip[4];   //Ŀ��IP��ַ
};
struct ArpPacket {
    EthernetHeader ed;
    ArpHeader ah;
};


void send(pcap_t* adhandle) {
    unsigned char sendbuf[42]; //arp���ṹ��С��42���ֽ�
    unsigned char mac[6] = { 0xf4,0x96,0x34,0x7f,0xa1,0xa7 };
    unsigned char ip[4] = {0xc0,0xa8,0x01,0x65};
    unsigned char ip_dist[4] = { 0xc0,0xa8,0x01,0x64 };

 
    EthernetHeader eh;
    ArpHeader ah;
    //��ֵMAC��ַ
    memset(eh.DestMAC, 0xff, 6);   //��̫���ײ�Ŀ��MAC��ַ��ȫΪ�㲥��ַ
    memcpy(eh.SourMAC, mac, 6);   //��̫���ײ�ԴMAC��ַ
    memcpy(ah.smac, mac, 6);   //ARP�ֶ�ԴMAC��ַ
    memset(ah.dmac, 0xff, 6);   //ARP�ֶ�Ŀ��MAC��ַ
    memcpy(ah.sip, ip, 4);   //ARP�ֶ�ԴIP��ַ
    memcpy(ah.dip, ip_dist, 4);   //ARP�ֶ�Ŀ��IP��ַ
    //memset(ah.dip, 0x05, 4);   //ARP�ֶ�Ŀ��IP��ַ
    eh.EthType = htons(ETH_ARP);   //htons�����������޷��Ŷ�������ת���������ֽ�˳��
    ah.hdtype = htons(ARP_HARDWARE);
    ah.protype = htons(ETH_IP);
    ah.hdsize = 6;
    ah.prosize = 4;
    ah.op = htons(ARP_REQUEST);
    memset(sendbuf, 0, sizeof(sendbuf));   //ARP����
    memcpy(sendbuf, &eh, sizeof(eh));
    memcpy(sendbuf + sizeof(eh), &ah, sizeof(ah));
    if (pcap_sendpacket(adhandle, sendbuf, 42) == 0) {
        printf("\nPacketSend succeed\n");
    }
    else {
        printf("PacketSendPacket in getmine Error: %d\n", GetLastError());
    }
    //192.168.1.101
    //f4:96 : 34 : 7f : a1 : a7

}

int main()
{
    pcap_if_t* alldevs;   //��������������  
    pcap_if_t* d;   //ѡ�е����������� 
    int inum;   //ѡ������������
    int i = 0;   //forѭ������
    pcap_t* adhandle;   //����������������׽ʵ��,��pcap_open���صĶ���
    char errbuf[PCAP_ERRBUF_SIZE];   //���󻺳���,��СΪ256
    int res;   //ץ������pcap_next_ex����ֵ��1-�ɹ���0����ȡ���ĳ�ʱ��-1����������-2: ��ȡ�����߼�¼�ļ������һ������
    u_int netmask;    //��������
    //ether proto protocol��������ݰ�����ĳЩ��̫Э�飨protocol������, ����˶�Ӧ���������ʽΪ�棬Э���ֶο�����ARP
    char packet_filter[] = "ether proto \\arp";   //Ҫץȡ�İ������ͣ�������ץȡARP����
    struct bpf_program fcode;   //pcap_compile�����õĽṹ��
    struct tm* ltime;   //��ʱ�䴦���йصı��� 
    char timestr[16];   //��ʱ�䴦���йصı���
    time_t local_tv_sec;    //��ʱ�䴦���йصı���
    struct pcap_pkthdr* header;   //���յ������ݰ���ͷ��
    const u_char* pkt_data;    //���յ������ݰ�������


    /* ��ȡ�����豸�б� */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* ��ӡ�б� */
    for (d = alldevs; d; d = d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }

    if (i == 0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return -1;
    }

    printf("Enter the interface number (1-%d):", i);
    scanf("%d", &inum);

    if (inum < 1 || inum > i)
    {
        printf("\nInterface number out of range.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* ��ת����ѡ�е������� */
    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

    /* ���豸 */
    if ((adhandle = pcap_open(d->name,          // �豸��
        65536,            // Ҫ��׽�����ݰ��Ĳ��� 
                          // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
        PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
        1000,             // ��ȡ��ʱʱ��
        NULL,             // Զ�̻�����֤
        errbuf            // ���󻺳��
    )) == NULL)
    {
        fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        /* �ͷ����б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* ���������·�㣬Ϊ�˼򵥣�����ֻ������̫�� */
    if (pcap_datalink(adhandle) != DLT_EN10MB)
    {
        fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (d->addresses != NULL)
        /* ��ýӿڵ�һ����ַ������ */
        netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* ����ӿ�û�е�ַ����ô���Ǽ���һ��C������� */
        netmask = 0xffffff;

    //���������
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
    {
        fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    //���ù�����
    if (pcap_setfilter(adhandle, &fcode) < 0)
    {
        fprintf(stderr, "\nError setting the filter.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("\nlistening on %s...\n", d->description);

    /* �ͷ��豸�б� */
    pcap_freealldevs(alldevs);

    /*���ϴ�����WinPcap�����ĵ��ж������ҵ�������ARP���Ĵ�����Ҫ�Լ���д*/

    /* ��ȡ���ݰ� */
    send(adhandle);
    
    while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0) {

        if (res == 0)
            /* ��ʱʱ�䵽 */
            continue;

       

        //����ARP��
        ArpHeader* arph = (ArpHeader*)(pkt_data + 14);

        //���� 
        printf("�������ͣ�");
        if (arph->op == 256)
            printf("������\t");
        else
            printf("Ӧ����\t");

        //����
        printf("����(B)��%d\t", header->len);

        //ʱ��
        /* ��ʱ���ת���ɿ�ʶ��ĸ�ʽ */
        local_tv_sec = header->ts.tv_sec;
        ltime = localtime(&local_tv_sec);
        strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

        printf("ʱ�䣺%s\n", timestr);


        //���ԴIP
        printf("ԴIP��");
        for (i = 0; i < 3; i++)
        {
            printf("%d.", arph->sip[i]);
        }
        printf("%d\t", arph->sip[3]);

        //���Ŀ��IP
        printf("Ŀ��IP��");
        for (i = 0; i < 3; i++)
        {
            printf("%d.", arph->dip[i]);

        }
        printf("%d\n", arph->dip[3]);

        //���ԴMAC
        printf("ԴMAC��");
        for (i = 0; i < 5; i++)
        {
            printf("%02x-", arph->smac[i]);
        }
        printf("%02x\t", arph->smac[5]);

        //���Ŀ��MAC
        printf("Ŀ��MAC��");
        for (i = 0; i < 5; i++)
        {
            printf("%02x-", *(pkt_data + i));
        }
        printf("%02x\n", *(pkt_data + 5));

        printf("---------------------------------------------------------\n");

    }

    if (res == -1) {   //����ARP������
        printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
        return -1;
    }

    return 0;
}