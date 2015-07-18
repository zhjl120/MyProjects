#include "stdafx.h"
#include "C_SockHelper.h"
#include <WinSock2.h>
//#include <Windows.h>
#include <IPHlpApi.h>
//#include <NtDDNdis.h>

bool C_SockHelper::InitWinSock()
{	
	WSADATA wsadata;

	if( WSAStartup(MAKEWORD(2,2), &wsadata)  == 0)
		return true;
	else
		return false;

}

void C_SockHelper::CleanupWinsock()
{
	WSACleanup();
}

bool C_SockHelper::GetLocalIP( __out char* pszIp )
{
	char szHostName[30];
	struct hostent *pHostent;

	if( gethostname((char*)szHostName, strlen(szHostName)) ==SOCKET_ERROR)
	{
		printf("%d", WSAGetLastError());
		return false;
	}
	if( (pHostent = gethostbyname(szHostName)) == NULL)
		return false;

	if(*pHostent->h_addr_list == NULL)
		return false;

	char *pIp  = inet_ntoa (*(struct in_addr *)pHostent->h_addr_list[0]);  
	strcpy(pszIp, pIp);

	return true;
}

bool C_SockHelper::GetLocalMac( __out char* pszMac )
{
	char szIP[20];

	if( GetLocalIP(szIP) == false)
		return false;

	if( GetMacByIP(szIP, pszMac) == false)
		return false;

	return true;
}

void C_SockHelper::GetGatewayIP( __out char* pszIp )
{
	char szIP[20];
	ULONG ipAdd = 0;
	u_long ulNetWork;

	if( GetLocalIP(szIP) == false)
		return;

	if( (ipAdd = inet_addr(szIP)) == INADDR_NONE)
		return;

	ulNetWork= ntohl(ipAdd);
	ulNetWork &= 0xffffff00;
	ipAdd = htonl(ulNetWork + 1);

	unsigned char *pIp = (unsigned char*)&ipAdd;
	sprintf(pszIp,"%d.%d.%d.%d",pIp[0],pIp[1],pIp[2],pIp[3]);
}

bool C_SockHelper::GetGatewayMac( __out char* pszMac )
{
	char szGatewayIp[20];
	GetGatewayIP(szGatewayIp);

	if(GetMacByIP(szGatewayIp, pszMac) == false)
		return false;

	return true;
}

bool C_SockHelper::GetMacByIP( __in char* pszIP, __out char* pszMac )
{
	IPAddr  ipAddr;
	ULONG   Mac[2];
	ULONG   MacLen;

	memset (Mac, 0xff, sizeof (Mac));
	MacLen = 6;

	ipAddr=inet_addr(pszIP);
	DWORD hr = SendARP (ipAddr, 0, Mac, &MacLen);
	if(hr != 0)
		return false;

	memcpy(pszMac, Mac, 6);

	//PBYTE  pbMac = (PBYTE)Mac;
	//for (ULONG i=0;i<MacLen;i++)
	//	pszMac[i]=pbMac[i];
	return true;
}

void C_SockHelper::GetBroadcastIP( __out char* pszIp )
{
	char szIP[20];
	ULONG ipAdd = 0;
	u_long ulNetWork;

	if( GetLocalIP(szIP) == false)
		return;

	if( (ipAdd = inet_addr(szIP)) == INADDR_NONE)
		return;

	ulNetWork= ntohl(ipAdd);
	ulNetWork &= 0xffffff00;
	ipAdd = htonl(ulNetWork + 255);

	unsigned char *pIp = (unsigned char*)&ipAdd;
	sprintf(pszIp,"%d.%d.%d.%d",pIp[0],pIp[1],pIp[2],pIp[3]);
}

void C_SockHelper::GetSubnetAdd( __out char* pszIP )
{
	char szIP[20];
	IPAddr  ipAddr;

	if( GetLocalIP(szIP) == false)
		return;

	ipAddr=inet_addr(szIP);
	unsigned char *pIp = (unsigned char*)&ipAddr;
	sprintf(pszIP,"%d.%d.%d",pIp[0],pIp[1],pIp[2]);
}

