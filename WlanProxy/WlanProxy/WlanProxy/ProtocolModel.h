#pragma once
#include "pcap.h"


/* 4字节的IP地址 */
typedef struct ip_address{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 首部 */
typedef struct ip_header{
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
}ip_header;

/* UDP 首部*/
typedef struct udp_header{
	u_short sport;          // 源端口(Source port)
	u_short dport;          // 目的端口(Destination port)
	u_short len;            // UDP数据包长度(Datagram length)
	u_short crc;            // 校验和(Checksum)
}udp_header;

/* TCP 首部*/
typedef struct tcp_header {
	USHORT nSourPort ;   // 源端口号16bit
	USHORT nDestPort ;   // 目的端口号16bit
	UINT nSequNum ;   // 序列号32bit
	UINT nAcknowledgeNum ; // 确认号32bit
	USHORT nHLenAndFlag ;  // 前4位：TCP头长度；中6位：保留；后6位：标志位: 共16bit
	USHORT nWindowSize ;  // 窗口大小16bit
	USHORT nCheckSum ;   // 检验和16bit
	USHORT nrgentPointer ;  // 紧急数据偏移量16bit
} tcp_header;

/*以太网首部*/
typedef struct et_header 
{
	UCHAR dhost[6]; //目的MAC地址
	UCHAR shost[6]; //源MAC地址
	USHORT type; //下层协议类型，如IP(ETHERTYPE_IP)，ARP（ETHERTYPE_ARP)等
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