#include "stdafx.h"
#include "C_SendPacket.h"
#include "C_SockHelper.h"

C_SendPacket::C_SendPacket( void )
{
	char szLocalIp[20];
	char error_info[LIBNET_ERRBUF_SIZE];		

	C_SockHelper::GetLocalIP(szLocalIp);
	m_pLibNet = libnet_init(LIBNET_LINK, szLocalIp, error_info);
	if(m_pLibNet == NULL)
		printf("libnet_init faild - error:%s", error_info);

	m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, _T(""));
}

C_SendPacket::~C_SendPacket( void )
{
	libnet_destroy(m_pLibNet);	
}

bool C_SendPacket::BulidArpPacket( char *srcIP, u_char *srcMac ,char *dstIP, u_char *dstMac )
{
	libnet_ptag_t protocol_tag;

	//把P地址字符串形式转化成网络顺序字节形式的数据
	u_long source_ip = libnet_name2addr4(m_pLibNet, srcIP, LIBNET_RESOLVE);
	u_long destination_ip = libnet_name2addr4(m_pLibNet, dstIP, LIBNET_RESOLVE);

	protocol_tag = libnet_build_arp(  //构造ARP协议块，函数的返回值是代表新生成的ARP协议块的一个协议块标记
		ARPHRD_ETHER,   // 硬件地址类型，在这里是以太网
		ETHERTYPE_IP,	 //协议地址类型，在这里是IP协议
		6,	  //硬件地址长度，MAC地址的长度为6
		4,	 //协议地址长度，IP地址的长度为4
		ARPOP_REPLY,	 //操作类型，在这里是ARP应答类型
		srcMac,	//源硬件地址
		(u_int8_t*)&source_ip,	 //源IP地址
		dstMac,	//目标硬件地址
		(u_int8_t*)&destination_ip,	 //目标协议地址
		NULL,  //负载，此时为NULL
		0,	 //负载的长度，此时为0
		m_pLibNet,	 //libnet句柄，此句柄由libnet_init()函数生成
		0	 //协议块标记，此时为0，表示构造一个新的ARP协议块，而不是修改已经存在的协议块
		);

	if(protocol_tag == -1)
		return false;

	protocol_tag = libnet_build_ethernet(  // 以auto的形式构造一个以太网协议块，返回一个指向此协议块的标记
		dstMac,	  //目的硬件地址
		srcMac,
		ETHERTYPE_ARP,	//以太网上层协议类型，此时为ARP类型
		NULL,
		0,
		m_pLibNet,	//libnet句柄
		0
		);
	
	if(protocol_tag == -1)
		return false;

	return true;
}

void C_SendPacket::SendArpPacket()
{
	while (::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0)
	{
		if( libnet_write(m_pLibNet) == -1)		//发送已经构造的ARP数据包
			printf("\nSend ARP Packet Error : libnet_write\n");

		Sleep(m_nSendInterval);
	}
}

DWORD __stdcall sendArpThread(LPVOID lpVoid)
{
	C_SendPacket* pThis = (C_SendPacket*)lpVoid;
	pThis->SendArpPacket();
	return 1;
}

void C_SendPacket::StartSendArp( char *srcIP, u_char *srcMac ,char *dstIP, u_char *dstMac, ULONG Interval )
{
	m_nSendInterval = Interval;

	if( BulidArpPacket(srcIP, srcMac, dstIP, dstMac) == false)
	{
		printf("\nBuild Arp Error! \n");
		return;
	}

	DWORD dwThreadID;
	m_hSendThread = ::CreateThread(NULL, 0, sendArpThread, this, 0, &dwThreadID);
}

void C_SendPacket::StopSendArp()
{
	DWORD dwExitCode = 0;

	::SetEvent(m_hStopEvent);
	while (1)
	{
		::GetExitCodeThread(m_hSendThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE )
			break;
		::Sleep(100);
	}
}

