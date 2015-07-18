#include "StdAfx.h"
#include "C_NetCmdFetchChannels.h"


C_NetCmdFetchChannels::C_NetCmdFetchChannels(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("request");
}

C_NetCmdFetchChannels::~C_NetCmdFetchChannels(void)
{
}
