#include "StdAfx.h"
#include "C_NetCmdLogOn.h"

C_NetCmdLogOn::C_NetCmdLogOn(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("request");
}

C_NetCmdLogOn::~C_NetCmdLogOn(void)
{
}

BOOL C_NetCmdLogOn::HandleRequest(LPCTSTR pszCmdStream, C_DBService& DBService)
{
	m_NetCmdDecomposer.Decompose(pszCmdStream);

/*	LPCTSTR pszID = m_NetCmdDecomposer.GetCmdPara(_T("id"));
	//check if the ID of device is registered
	if(false)
	{
		m_szResult = _T("fail");
	}
	else
	{
		LPCTSTR pszRecentlyReq = m_NetCmdDecomposer.GetCmdPara(_T("recently request"));
		//check recently request time
		if (false)
		{
			m_szResult = _T("noupdate");
		}
		else
		{
			m_szResult = _T("success");
		}
	}
*/
	return TRUE;
}

LPCTSTR C_NetCmdLogOn::GetID()
{
	LPCTSTR pszID;
	m_NetCmdDecomposer.GetCmdPara(_T("id"), pszID);

	return pszID;
}
