/*
 * Copyright (c) 2005 - 2006
 *		CACE Technologies, Davis, CA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the 
 * documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of CACE Technologies nor the names of its 
 * contributors may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/*
 * This simple program implements a user-level bridge. 
 * It opens two adapters specified by the user and starts two threads.
 * The first thread receives packets from adapter 1 and sends them down to
 * adapter 2. The second thread does the same, but in the opposite 
 * direction.
 */

#include <signal.h>
#include "pcap.h"


/* Storage data structure used to pass parameters to the threads */
typedef struct _in_out_adapters
{
	unsigned int state;		/* Some simple state information */
	pcap_t *input_adapter;
	pcap_t *output_adapter;
	unsigned int input_id;
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


/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* 6 bytes Mac address */
typedef struct mac_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
	u_char byte5;
	u_char byte6;
}mac_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	u_int	op_pad;			// Option + Padding
}ip_header;

/* Mac header */
typedef struct mac_header
{
	mac_address	daddr;		// Destination address
	mac_address	saddr;		// Source 
	u_int	type;			// Type
}mac_header;


/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;

/*******************************************************************/

void print_table();
typedef struct learning_table_item
{
	mac_address dst_mac;
	unsigned int port_id;
}learning_table_item;

typedef struct port_item
{
	pcap_t* port;
	int port_id;
}port_item;

struct learning_table_item learning_table[100];
struct port_item port_table[2];
int table_index = 0;	//����ָ��

void init_table() {
	memset(learning_table, 0, sizeof(learning_table));
	printf("���ѧϰ��\n");
	Sleep(3000);
}

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

	/* 
	 * Retrieve the device list 
	 */

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	
	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
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
	printf("\nSpecify filter (hit return for no filter):");
	
	fgets(packet_filter, sizeof(packet_filter), stdin);

	/* Get the first interface number*/
	printf("\nEnter the number of the first interface to use (1-%d):",i);
	scanf_s("%d", &inum1);
	
	if(inum1 < 1 || inum1 > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Get the second interface number*/
	printf("Enter the number of the first interface to use (1-%d):",i);
	scanf_s("%d", &inum2);
	
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
	couple0.input_id= inum1;
	couple0.input_adapter = adhandle1;
	couple0.output_adapter = adhandle2;
	couple1.state = 1;
	couple1.input_id = inum2;
	couple1.input_adapter = adhandle2;
	couple1.output_adapter = adhandle1;


	//�����2�ǳ�������������ֻ������adapter
	port_table[0].port_id = inum1;
	port_table[1].port_id = inum2;
	port_table[0].port = adhandle1;
	port_table[1].port = adhandle2;
	
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
	in_out_adapters* ad_couple = lpParameter;
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
			
			if (ad_couple->state == 0) {
				printf("�������� %d ��������֡", ad_couple->state);printf("\t>> Len: %u\n", header->caplen);
			}
			else {
				printf("�������� %d ��������֡", ad_couple->state);printf("\t<< Len: %u\n", header->caplen);
			}
			
			LeaveCriticalSection(&print_cs); 

			ip_header* ih;
			mac_header* mh;

			mh = (mac_header*)(pkt_data);
			ih = (ip_header*)(pkt_data + 14); //length of ethernet header

			

			printf("%X:%X:%X:%X:%X:%X -> %X:%X:%X:%X:%X:%X\n",
				mh->saddr.byte1,
				mh->saddr.byte2,
				mh->saddr.byte3,
				mh->saddr.byte4,
				mh->saddr.byte5,
				mh->saddr.byte6,
				mh->daddr.byte1,
				mh->daddr.byte2,
				mh->daddr.byte3,
				mh->daddr.byte4,
				mh->daddr.byte5,
				mh->daddr.byte6);
			printf("%d.%d.%d.%d -> %d.%d.%d.%d\n",
				ih->saddr.byte1,
				ih->saddr.byte2,
				ih->saddr.byte3,
				ih->saddr.byte4,
				ih->daddr.byte1,
				ih->daddr.byte2,
				ih->daddr.byte3,
				ih->daddr.byte4);

			int flag = check_table(mh->saddr);
			if (flag == -1) {
				learning_table[table_index].dst_mac = mh->saddr;
				learning_table[table_index++].port_id = ad_couple->input_id;
			}

			print_table();

			



			// ����
			flag = check_table(mh->daddr);
			if (flag == -1) {
				// ����
				for (int i = 0; i < 2; ++i) {
					if (ad_couple->input_id!=port_table[i].port_id) {
						/*
					 * Send the just received packet to the output adaper
					*/
						printf("flooding \n");
						if (pcap_sendpacket(port_table[i].port, pkt_data, header->caplen) != 0)
						{
							EnterCriticalSection(&print_cs);

							printf("Error sending a %u bytes packets on interface %u: %s\n",
								header->caplen,
								ad_couple->state,
								pcap_geterr(port_table[i].port));

							LeaveCriticalSection(&print_cs);
						}
						else
						{
							n_fwd++;
						}
					}
				}
			}
			else {
				if (flag == ad_couple->input_id) {
					printf("Ŀ�ĵ�ַ�Ķ˿���Դ�˿���ͬ��������֡\n");
				}
				else {
					/*
					 * Send the just received packet to the output adaper
					*/
					if (pcap_sendpacket(ad_couple->output_adapter, pkt_data, header->caplen) != 0)
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

			if (table_index > 5) {
				init_table();
				table_index = 0;
			}

			printf("-----------------\n");
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

void print_table() {
	printf("\tѧϰ�����\n");
	for (int i = 0; i < table_index; ++i) {
		printf("%d\t%X:%X:%X:%X:%X:%X\t%d\n",i,
			learning_table[i].dst_mac.byte1,
			learning_table[i].dst_mac.byte2,
			learning_table[i].dst_mac.byte3,
			learning_table[i].dst_mac.byte4,
			learning_table[i].dst_mac.byte5,
			learning_table[i].dst_mac.byte6,
			learning_table[i].port_id);
	}
}

int check_table(mac_address input_name) {
	for (int i = 0; i < table_index; ++i) {
		/*if ((strcmp(input_name.byte1,learning_table[i].dst_mac.byte1)==0)
			&& (strcmp(input_name.byte2, learning_table[i].dst_mac.byte2 ) == 0)
			&& (strcmp(input_name.byte3, learning_table[i].dst_mac.byte3 ) == 0)
			&& (strcmp(input_name.byte4, learning_table[i].dst_mac.byte4 ) == 0)
			&& (strcmp(input_name.byte5, learning_table[i].dst_mac.byte5 ) == 0)
			&& (strcmp(input_name.byte6, learning_table[i].dst_mac.byte6 ) == 0)
			) {*/
		if ((input_name.byte1== learning_table[i].dst_mac.byte1)
			&& (input_name.byte2== learning_table[i].dst_mac.byte2) 
			&& (input_name.byte3== learning_table[i].dst_mac.byte3) 
			&& (input_name.byte4== learning_table[i].dst_mac.byte4) 
			&& (input_name.byte5== learning_table[i].dst_mac.byte5) 
			&& (input_name.byte6== learning_table[i].dst_mac.byte6) 
			) {
			return learning_table[i].port_id;
		}
	}

	return -1;
}

