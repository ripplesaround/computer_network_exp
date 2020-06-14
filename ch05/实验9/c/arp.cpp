#include "stdio.h"
#include "pcap.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#define ETH_ARP         0x0806  //以太网帧类型表示后面数据的类型，对于ARP请求或应答来说，该字段的值为x0806
#define ARP_HARDWARE    1  //硬件类型字段值为表示以太网地址
#define ETH_IP          0x0800  //协议类型字段表示要映射的协议地址类型值为x0800表示IP地址
#define ARP_REQUEST     1   //ARP请求
#define ARP_RESPONSE       2      //ARP应答


struct EthernetHeader
{
    u_char DestMAC[6];    //目的MAC地址 6字节
    u_char SourMAC[6];   //源MAC地址 6字节
    u_short EthType;         //上一层协议类型，如0x0800代表上一层是IP协议，0x0806为arp  2字节
};

struct ArpHeader
{
    unsigned short hdtype;   //硬件类型
    unsigned short protype;   //协议类型
    unsigned char hdsize;   //硬件地址长度
    unsigned char prosize;   //协议地址长度
    unsigned short op;   //操作类型，ARP请求（1），ARP应答（2）
    u_char smac[6];   //源MAC地址
    u_char sip[4];   //源IP地址
    u_char dmac[6];   //目的MAC地址
    u_char dip[4];   //目的IP地址
};
struct ArpPacket {
    EthernetHeader ed;
    ArpHeader ah;
};


void send(pcap_t* adhandle) {
    unsigned char sendbuf[42]; //arp包结构大小，42个字节
    unsigned char mac[6] = { 0xf4,0x96,0x34,0x7f,0xa1,0xa7 };
    unsigned char ip[4] = {0xc0,0xa8,0x01,0x65};
    unsigned char ip_dist[4] = { 0xc0,0xa8,0x01,0x64 };

 
    EthernetHeader eh;
    ArpHeader ah;
    //赋值MAC地址
    memset(eh.DestMAC, 0xff, 6);   //以太网首部目的MAC地址，全为广播地址
    memcpy(eh.SourMAC, mac, 6);   //以太网首部源MAC地址
    memcpy(ah.smac, mac, 6);   //ARP字段源MAC地址
    memset(ah.dmac, 0xff, 6);   //ARP字段目的MAC地址
    memcpy(ah.sip, ip, 4);   //ARP字段源IP地址
    memcpy(ah.dip, ip_dist, 4);   //ARP字段目的IP地址
    //memset(ah.dip, 0x05, 4);   //ARP字段目的IP地址
    eh.EthType = htons(ETH_ARP);   //htons：将主机的无符号短整形数转换成网络字节顺序
    ah.hdtype = htons(ARP_HARDWARE);
    ah.protype = htons(ETH_IP);
    ah.hdsize = 6;
    ah.prosize = 4;
    ah.op = htons(ARP_REQUEST);
    memset(sendbuf, 0, sizeof(sendbuf));   //ARP清零
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
    pcap_if_t* alldevs;   //所有网络适配器  
    pcap_if_t* d;   //选中的网络适配器 
    int inum;   //选择网络适配器
    int i = 0;   //for循环变量
    pcap_t* adhandle;   //打开网络适配器，捕捉实例,是pcap_open返回的对象
    char errbuf[PCAP_ERRBUF_SIZE];   //错误缓冲区,大小为256
    int res;   //抓包函数pcap_next_ex返回值，1-成功、0：获取报文超时、-1：发生错误、-2: 获取到离线记录文件的最后一个报文
    u_int netmask;    //子网掩码
    //ether proto protocol：如果数据包属于某些以太协议（protocol）类型, 则与此对应的条件表达式为真，协议字段可以是ARP
    char packet_filter[] = "ether proto \\arp";   //要抓取的包的类型，这里是抓取ARP包；
    struct bpf_program fcode;   //pcap_compile所调用的结构体
    struct tm* ltime;   //和时间处理有关的变量 
    char timestr[16];   //和时间处理有关的变量
    time_t local_tv_sec;    //和时间处理有关的变量
    struct pcap_pkthdr* header;   //接收到的数据包的头部
    const u_char* pkt_data;    //接收到的数据包的内容


    /* 获取本机设备列表 */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* 打印列表 */
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
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* 跳转到已选中的适配器 */
    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

    /* 打开设备 */
    if ((adhandle = pcap_open(d->name,          // 设备名
        65536,            // 要捕捉的数据包的部分 
                          // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
        PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
        1000,             // 读取超时时间
        NULL,             // 远程机器验证
        errbuf            // 错误缓冲池
    )) == NULL)
    {
        fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
        /* 释放设列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* 检查数据链路层，为了简单，我们只考虑以太网 */
    if (pcap_datalink(adhandle) != DLT_EN10MB)
    {
        fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (d->addresses != NULL)
        /* 获得接口第一个地址的掩码 */
        netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* 如果接口没有地址，那么我们假设一个C类的掩码 */
        netmask = 0xffffff;

    //编译过滤器
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0)
    {
        fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    //设置过滤器
    if (pcap_setfilter(adhandle, &fcode) < 0)
    {
        fprintf(stderr, "\nError setting the filter.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("\nlistening on %s...\n", d->description);

    /* 释放设备列表 */
    pcap_freealldevs(alldevs);

    /*以上代码在WinPcap开发文档中都可以找到，解析ARP包的代码则要自己编写*/

    /* 获取数据包 */
    send(adhandle);
    
    while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0) {

        if (res == 0)
            /* 超时时间到 */
            continue;

       

        //解析ARP包
        ArpHeader* arph = (ArpHeader*)(pkt_data + 14);

        //类型 
        printf("报文类型：");
        if (arph->op == 256)
            printf("请求报文\t");
        else
            printf("应答报文\t");

        //长度
        printf("长度(B)：%d\t", header->len);

        //时间
        /* 将时间戳转换成可识别的格式 */
        local_tv_sec = header->ts.tv_sec;
        ltime = localtime(&local_tv_sec);
        strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

        printf("时间：%s\n", timestr);


        //输出源IP
        printf("源IP：");
        for (i = 0; i < 3; i++)
        {
            printf("%d.", arph->sip[i]);
        }
        printf("%d\t", arph->sip[3]);

        //输出目的IP
        printf("目的IP：");
        for (i = 0; i < 3; i++)
        {
            printf("%d.", arph->dip[i]);

        }
        printf("%d\n", arph->dip[3]);

        //输出源MAC
        printf("源MAC：");
        for (i = 0; i < 5; i++)
        {
            printf("%02x-", arph->smac[i]);
        }
        printf("%02x\t", arph->smac[5]);

        //输出目的MAC
        printf("目的MAC：");
        for (i = 0; i < 5; i++)
        {
            printf("%02x-", *(pkt_data + i));
        }
        printf("%02x\n", *(pkt_data + 5));

        printf("---------------------------------------------------------\n");

    }

    if (res == -1) {   //接收ARP包出错
        printf("Error reading the packets: %s\n", pcap_geterr(adhandle));
        return -1;
    }

    return 0;
}