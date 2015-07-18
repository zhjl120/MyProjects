#include "StdAfx.h"
#include "C_NetCmdLogOff.h"
#include "C_ClientMac2Socket.h"
#include "C_NetCmdStatistics.h"


C_NetCmdLogOff::C_NetCmdLogOff()
{
	m_szCmdName = _T("log off");
}

C_NetCmdLogOff::~C_NetCmdLogOff(void)
{
}

BOOL C_NetCmdLogOff::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if(! DBOperate.LogOff(pszMac))
		return FALSE;

	C_NetLoginStatistics::UpdateOnlineTime(DBOperate,pszMac);
	C_ClientMac2Socket::GetInstance()->RemoveClient(pszMac);
	return TRUE;
}
