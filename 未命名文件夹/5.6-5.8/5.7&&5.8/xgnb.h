#pragma once

#include <windows.h>

#pragma pack(1)
//IP���ݱ�ͷ
typedef struct
{
    unsigned char hdr_len : 4;  // 4λ�ײ�����
    unsigned char version : 4;  // 4λ�汾��
    unsigned char tos;   // 8λ��������
    unsigned short total_len;  // 16λ�ܳ���
    unsigned short identifier;  // 16λ��ʶλ
    unsigned short frag_and_flags; // flags
    unsigned char ttl;   // 8λ����ʱ�䣨TTL��
    unsigned char protocol;  // 8λ�ϲ�Э��
    unsigned short checksum;  // 16λУ���
    unsigned long sourceIP;  // ԴIP��ַ
    unsigned long destIP;   // Ŀ��IP��ַ
} IP_HEADER;

//ICMP���ݱ�ͷ
typedef struct
{
    BYTE type;  //8λ����
    BYTE code;  //8λ����
    USHORT cksum;  //16λУ���
    USHORT id;   //16λ��ʶ��
    USHORT seq;  //16λ���к�
} ICMP_HEADER;

//������
typedef struct
{
    USHORT usSeqNo;   //�����к�
    DWORD dwRoundTripTime; //����ʱ��
    in_addr dwIPaddr;  //�Զ�IP��ַ
} DECODE_RESULT;
#pragma pack()

const BYTE ICMP_ECHO_REQUEST = 8; //�������
const BYTE ICMP_ECHO_REPLY = 0; //����Ӧ��
const BYTE ICMP_TIMEOUT = 11; //���䳬ʱ

const int DEF_MAX_HOP = 30;    //�����վ��
const DWORD DEF_ICMP_TIMEOUT = 3000; //Ĭ�ϳ�ʱʱ�䣬��λms
const int DEF_ICMP_DATA_SIZE = 32; //Ĭ��ICMP���ݲ��ֳ���
const int MAX_ICMP_PACKET_SIZE = 1024; //���ICMP���ݱ��Ĵ�С

USHORT GenerateChecksum(USHORT* pBuf, int iSize);
BOOL DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& stDecodeResult);
