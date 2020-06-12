#include <signal.h>
#include "pcap.h"
#include <stdio.h>
#define HAVE_REMOTE
#define LINE_LEN 16
#include "winsock.h"
#include <string.h>
#include <remote-ext.h>

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



/* Storage data structure used to pass parameters to the threads */
typedef struct _in_out_adapters
{
	unsigned int state;		/* Some simple state information */
	pcap_t *input_adapter;
	pcap_t *output_adapter;
}in_out_adapters;

/* Prototypes */
DWORD WINAPI CaptureAndForwardThread(LPVOID lpParameter);
void ctrlc_handler(int sig);

/* This prevents the two threads to mess-up when they do printfs */
CRITICAL_SECTION print_cs;

/* Thread handlers. Global because we wait on the threads from the CTRL+C handler */
HANDLE threads[2];

/* This global variable tells the forwarder threads they must terminate */
volatile int kill_forwaders = 0;


/*******************************************************************/

int main()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum1, inum2;
	int i=0;
	pcap_t *adhandle1, *adhandle2;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask1, netmask2;
	char packet_filter[256];
	struct bpf_program fcode;
	in_out_adapters couple0, couple1;
	pcap_t * adhandle;
	
		printf("��������ѧ���������ʵ��--2020.6\n"); 
    	printf("�����±��ʵ��5.6-NAT by WinPcap and DEV-C++\n"); 
    	printf("Author��ZHC LX HYF\n");
    //	printf("%x %x \n",192,168);

	/* 
	 * Retrieve the device list 
	 */

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		fprintf(stderr,"�޷��������豸:%s\n", errbuf);
		exit(1);
	}
	
	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		if (i == 0)
		{
			printf("�������� ѧ�Ծ���\n");
			printf("�����豸����:\n");
		}
		printf("%d. ", ++i);
		if (d->description)
			printf("%s\n", d->description);
		else
			printf("<unknown adapter>\n");
			
	}
	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	/*
	 * Get input from the user
	 */

	/* Get the filter*/
	printf("\nSpecify filter -- hit return to continus:");
	
	fgets(packet_filter, sizeof(packet_filter), stdin);

	/* Get the first interface number*/
	printf("\nEnter the number of the Intranet to use (1-%d):",i);
	scanf("%d", &inum1);
	
	if(inum1 < 1 || inum1 > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Get the second interface number*/
	printf("Enter the number of the Extranet to use (1-%d):",i);
	scanf("%d", &inum2);
	
	if(inum2 < 1 || inum2 > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	if(inum1 == inum2 )
	{
		printf("\nCannot bridge packets on the same interface.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}


	/*
	 * Open the specified couple of adapters
	 */

	/* Jump to the first selected adapter */
	for(d = alldevs, i = 0; i< inum1 - 1 ;d = d->next, i++);
	
	/* 
	 * Open the first adapter.
	 * *NOTICE* the flags we are using, they are important for the behavior of the prgram:
	 *	- PCAP_OPENFLAG_PROMISCUOUS: tells the adapter to go in promiscuous mode.
	 *    This means that we are capturing all the traffic, not only the one to or from
	 *    this machine.
	 *	- PCAP_OPENFLAG_NOCAPTURE_LOCAL: prevents the adapter from capturing again the packets
	 *	  transmitted by itself. This avoids annoying loops.
	 *	- PCAP_OPENFLAG_MAX_RESPONSIVENESS: configures the adapter to provide minimum latency,
	 *	  at the cost of higher CPU usage.
	 */
	if((adhandle1 = pcap_open(d->name,						// name of the device
							 65536,							// portion of the packet to capture. 
															// 65536 grants that the whole packet will be captured on every link layer.
							 PCAP_OPENFLAG_PROMISCUOUS |	// flags. We specify that we don't want to capture loopback packets, and that the driver should deliver us the packets as fast as possible
							 PCAP_OPENFLAG_NOCAPTURE_LOCAL |
							 PCAP_OPENFLAG_MAX_RESPONSIVENESS,
							 500,							// read timeout
							 NULL,							// remote authentication
							 errbuf							// error buffer
							 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->description);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
		
	if(d->addresses != NULL)
	{
		/* Retrieve the mask of the first address of the interface */
		netmask1 = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask1 = 0xffffff; 
	}

	/* Jump to the second selected adapter */
	for(d = alldevs, i = 0; i< inum2 - 1 ;d = d->next, i++);
	
	/* Open the second adapter */
	if((adhandle2 = pcap_open(d->name,						// name of the device
							 65536,							// portion of the packet to capture. 
															// 65536 grants that the whole packet will be captured on every link layer.
							 PCAP_OPENFLAG_PROMISCUOUS |	// flags. We specify that we don't want to capture loopback packets, and that the driver should deliver us the packets as fast as possible
							 PCAP_OPENFLAG_NOCAPTURE_LOCAL |
							 PCAP_OPENFLAG_MAX_RESPONSIVENESS,
							 500,							// read timeout
							 NULL,							// remote authentication
							 errbuf							// error buffer
							 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->description);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	if(d->addresses != NULL)
	{
		/* Retrieve the mask of the first address of the interface */
		netmask2 = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask2 = 0xffffff; 
	}


	/*
	 * Compile and set the filters
	 */

	/* compile the filter for the first adapter */
	if (pcap_compile(adhandle1, &fcode, packet_filter, 1, netmask1) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");

		/* Close the adapters */
		pcap_close(adhandle1);
		pcap_close(adhandle2);

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* set the filter for the first adapter*/
	if (pcap_setfilter(adhandle1, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");

		/* Close the adapters */
		pcap_close(adhandle1);
		pcap_close(adhandle2);

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* compile the filter for the second adapter */
	if (pcap_compile(adhandle2, &fcode, packet_filter, 1, netmask2) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		
		/* Close the adapters */
		pcap_close(adhandle1);
		pcap_close(adhandle2);

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* set the filter for the second adapter*/
	if (pcap_setfilter(adhandle2, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");

		/* Close the adapters */
		pcap_close(adhandle1);
		pcap_close(adhandle2);

		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* At this point, we don't need the device list any more. Free it */
	pcap_freealldevs(alldevs);
		
	/* 
	 * Start the threads that will forward the packets 
	 */

	/* Initialize the critical section that will be used by the threads for console output */
	InitializeCriticalSection(&print_cs);

	/* Init input parameters of the threads */
	couple0.state = 0;
	couple0.input_adapter = adhandle1;
	couple0.output_adapter = adhandle2;
	couple1.state = 1;
	couple1.input_adapter = adhandle2;
	couple1.output_adapter = adhandle1;
	
	/* Start first thread */
	if((threads[0] = CreateThread(
		NULL,
		0,
		CaptureAndForwardThread,
		&couple0,
		0,
		NULL)) == NULL)
	{
			fprintf(stderr, "error creating the first forward thread");
			
			/* Close the adapters */
			pcap_close(adhandle1);
			pcap_close(adhandle2);
			
			/* Free the device list */
			pcap_freealldevs(alldevs);
			return -1;
	}

	/* Start second thread */
	if((threads[1] = CreateThread(
		NULL,
		0,
		CaptureAndForwardThread,
		&couple1,
		0,
		NULL)) == NULL)
	{
			fprintf(stderr, "error creating the second forward thread");
	
			/* Kill the first thread. Not very gentle at all...*/
			TerminateThread(threads[0], 0);

			/* Close the adapters */
			pcap_close(adhandle1);
			pcap_close(adhandle2);
			
			/* Free the device list */
			pcap_freealldevs(alldevs);
			return -1;
	}

	/*
	 * Install a CTRL+C handler that will do the cleanups on exit
	 */
	signal(SIGINT, ctrlc_handler);

	/* 
	 * Done! 
	 * Wait for the Greek calends... 
	 */
	printf("\nStart bridging the two adapters...\n", d->description);
	pcap_loop(adhandle1, 0, packet_handler, NULL);
	Sleep(INFINITE);
	return 0;
}

/*******************************************************************
 * Forwarding thread.
 * Gets the packets from the input adapter and sends them to the output one.
 *******************************************************************/
DWORD WINAPI CaptureAndForwardThread(LPVOID lpParameter)
{
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int res = 0;
	in_out_adapters* ad_couple = (in_out_adapters*)lpParameter;
	unsigned int n_fwd = 0;
	
	/*
	 * Loop receiving packets from the first input adapter
	 */

	while((!kill_forwaders) && (res = pcap_next_ex(ad_couple->input_adapter, &header, &pkt_data)) >= 0)
	{		
		if(res != 0)	/* Note: res=0 means "read timeout elapsed"*/
		{
			/* 
			 * Print something, just to show when we have activity.
			 * BEWARE: acquiring a critical section and printing strings with printf
			 * is something inefficient that you seriously want to avoid in your packet loop!	
			 * However, since this is a *sample program*, we privilege visual output to efficiency.
			 */
			EnterCriticalSection(&print_cs);
			
			if(ad_couple->state == 0)
				int haha = 0;
		//		printf(">> Len: %u\n", header->caplen);
			else
				int hehe =0;
		//		printf("<< Len: %u\n", header->caplen);		
			
			LeaveCriticalSection(&print_cs); 

			/*
			 * Send the just received packet to the output adaper
			 */
			if(pcap_sendpacket(ad_couple->output_adapter, pkt_data, header->caplen) != 0)
			{
				EnterCriticalSection(&print_cs);
				
				printf("Error sending a %u bytes packets on interface %u: %s\n",
					header->caplen,
					ad_couple->state,
					pcap_geterr(ad_couple->output_adapter));
				
				LeaveCriticalSection(&print_cs); 
			}
			else
			{
				n_fwd++;
			}
		}
	}

	/*
	 * We're out of the main loop. Check the reason.
	 */
	if(res < 0)
	{
		EnterCriticalSection(&print_cs);

		printf("Error capturing the packets: %s\n", pcap_geterr(ad_couple->input_adapter));
		fflush(stdout);
		
		LeaveCriticalSection(&print_cs); 
	}
	else
	{
		EnterCriticalSection(&print_cs);

		printf("End of bridging on interface %u. Forwarded packets:%u\n",
			ad_couple->state,
			n_fwd);
		fflush(stdout);
		
		LeaveCriticalSection(&print_cs);
	}

	return 0;
}

/*******************************************************************
 * CTRL+C hanlder.
 * We order the threads to die and then we patiently wait for their
 * suicide.
 *******************************************************************/
void ctrlc_handler(int sig)
{
	/*
	 * unused variable
	 */
	(VOID)(sig);

	kill_forwaders = 1;

	WaitForMultipleObjects(2,
				threads,
				TRUE,		/* Wait for all the handles */
				5000);		/* Timeout */

	exit(0);
}
