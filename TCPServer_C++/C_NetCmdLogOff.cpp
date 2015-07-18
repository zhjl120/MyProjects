#include "StdAfx.h"
#include "C_NetCmdLogOff.h"

C_NetCmdLogOff::C_NetCmdLogOff(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("request");
}


C_NetCmdLogOff::~C_NetCmdLogOff(void)
{
}

BOOL C_NetCmdLogOff::HandleRequest(LPCTSTR pszCmdStream, C_DBService& DBService)
{
	m_NetCmdDecomposer.Decompose(pszCmdStream);

	//change device online status to 0

	return TRUE;
}
