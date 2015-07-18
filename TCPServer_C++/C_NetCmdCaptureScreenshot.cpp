#include "stdafx.h"
#include "C_NetCmdCaptureScreenshot.h"
#include "C_ClientMac2Socket.h"

C_NetCmdCaptureScreenshot::C_NetCmdCaptureScreenshot()
{
}

C_NetCmdCaptureScreenshot::~C_NetCmdCaptureScreenshot( void )
{
}

BOOL C_NetCmdCaptureScreenshot::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, 
	C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	tstring szCmdCapture = _T("{\"type\":\"request\",\"cmd\":\"capture screenshot\", \"parameters\":null}\n");

	if(! m_NetCmdDecomposer.Decompose(pszCmdStream) )
		return FALSE;

	for (DWORD dwCount = 0 ; dwCount < m_NetCmdDecomposer.GetArraySize(_T("idArray")) ; dwCount ++)
	{
		tstring szMac = m_NetCmdDecomposer.GetValueByIndex(_T("idArray"), dwCount);

		if (szMac.compare(_T("")) == 0)
			continue;

		Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr(szMac.c_str());
		if (psktClient != NULL)
			psktClient->sendBytes(szCmdCapture.c_str(),szCmdCapture.length());
		//{
		//	SocketList ReadList, WriteList, ExceptList;
		//	WriteList.push_back(sktClient);
		//	if (Poco::Net::Socket::select(ReadList, WriteList, ExceptList, Poco::Timespan(10,0)) > 0)
		//	{
		//		sktClient->sendBytes(szCmdCapture.c_str(),szCmdCapture.length());
		//	}
		//	WriteList.clear();
		//}	
	}

	return TRUE;
}

