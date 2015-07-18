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

	//��P��ַ�ַ�����ʽת��������˳���ֽ���ʽ������
	u_long source_ip = libnet_name2addr4(m_pLibNet, srcIP, LIBNET_RESOLVE);
	u_long destination_ip = libnet_name2addr4(m_pLibNet, dstIP, LIBNET_RESOLVE);

	protocol_tag = libnet_build_arp(  //����ARPЭ��飬�����ķ���ֵ�Ǵ��������ɵ�ARPЭ����һ��Э�����
		ARPHRD_ETHER,   // Ӳ����ַ���ͣ�����������̫��
		ETHERTYPE_IP,	 //Э���ַ���ͣ���������IPЭ��
		6,	  //Ӳ����ַ���ȣ�MAC��ַ�ĳ���Ϊ6
		4,	 //Э���ַ���ȣ�IP��ַ�ĳ���Ϊ4
		ARPOP_REPLY,	 //�������ͣ���������ARPӦ������
		srcMac,	//ԴӲ����ַ
		(u_int8_t*)&source_ip,	 //ԴIP��ַ
		dstMac,	//Ŀ��Ӳ����ַ
		(u_int8_t*)&destination_ip,	 //Ŀ��Э���ַ
		NULL,  //���أ���ʱΪNULL
		0,	 //���صĳ��ȣ���ʱΪ0
		m_pLibNet,	 //libnet������˾����libnet_init()��������
		0	 //Э����ǣ���ʱΪ0����ʾ����һ���µ�ARPЭ��飬�������޸��Ѿ����ڵ�Э���
		);

	if(protocol_tag == -1)
		return false;

	protocol_tag = libnet_build_ethernet(  // ��auto����ʽ����һ����̫��Э��飬����һ��ָ���Э���ı��
		dstMac,	  //Ŀ��Ӳ����ַ
		srcMac,
		ETHERTYPE_ARP,	//��̫���ϲ�Э�����ͣ���ʱΪARP����
		NULL,
		0,
		m_pLibNet,	//libnet���
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
		if( libnet_write(m_pLibNet) == -1)		//�����Ѿ������ARP���ݰ�
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

