#pragma once
#include "pcap.h"


/* 4�ֽڵ�IP��ַ */
typedef struct ip_address{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 �ײ� */
typedef struct ip_header{
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
}ip_header;

/* UDP �ײ�*/
typedef struct udp_header{
	u_short sport;          // Դ�˿�(Source port)
	u_short dport;          // Ŀ�Ķ˿�(Destination port)
	u_short len;            // UDP���ݰ�����(Datagram length)
	u_short crc;            // У���(Checksum)
}udp_header;

/* TCP �ײ�*/
typedef struct tcp_header {
	USHORT nSourPort ;   // Դ�˿ں�16bit
	USHORT nDestPort ;   // Ŀ�Ķ˿ں�16bit
	UINT nSequNum ;   // ���к�32bit
	UINT nAcknowledgeNum ; // ȷ�Ϻ�32bit
	USHORT nHLenAndFlag ;  // ǰ4λ��TCPͷ���ȣ���6λ����������6λ����־λ: ��16bit
	USHORT nWindowSize ;  // ���ڴ�С16bit
	USHORT nCheckSum ;   // �����16bit
	USHORT nrgentPointer ;  // ��������ƫ����16bit
} tcp_header;

/*��̫���ײ�*/
typedef struct et_header 
{
	UCHAR dhost[6]; //Ŀ��MAC��ַ
	UCHAR shost[6]; //ԴMAC��ַ
	USHORT type; //�²�Э�����ͣ���IP(ETHERTYPE_IP)��ARP��ETHERTYPE_ARP)��
}et_header;

struct dns_header
{
//	uint16_t h_len;          /* length of the packet - only used with TCP */
	uint16_t id;             /* DNS packet ID */
	uint16_t flags;          /* DNS flags */
	uint16_t num_q;          /* Number of questions */
	uint16_t num_answ_rr;    /* Number of answer resource records */
	uint16_t num_auth_rr;    /* Number of authority resource records */
	uint16_t num_addi_rr;    /* Number of additional resource records */
};