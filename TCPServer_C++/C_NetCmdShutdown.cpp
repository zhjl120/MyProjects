#include "stdafx.h"
#include "C_NetCmdShutdown.h"
#include "C_ClientMac2Socket.h"

C_NetCmdShutdown::C_NetCmdShutdown()
{
}

C_NetCmdShutdown::~C_NetCmdShutdown( void )
{
}

BOOL C_NetCmdShutdown::HandleRequest( Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, 
	C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	tstring szCmd = _T("{\"type\":\"request\",\"cmd\":\"shutdown\", \"parameters\":null}\n");

	if(! m_NetCmdDecomposer.Decompose(pszCmdStream) )
		return FALSE;

	for (DWORD dwCount = 0 ; dwCount < m_NetCmdDecomposer.GetArraySize(_T("idArray")) ; dwCount ++)
	{
		tstring szMac = m_NetCmdDecomposer.GetValueByIndex(_T("idArray"), dwCount);

		if (szMac.compare(_T("")) == 0)
			continue;

		Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr(szMac.c_str());
		if (psktClient != NULL)
			psktClient->sendBytes(szCmd.c_str(),szCmd.length());
	}

	return TRUE;
}
