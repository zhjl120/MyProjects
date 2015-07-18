#include "StdAfx.h"
#include "C_NetCmdFetchChannelContent.h"

C_NetCmdFetchChannelContent::C_NetCmdFetchChannelContent(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType = _T("request");
}

C_NetCmdFetchChannelContent::~C_NetCmdFetchChannelContent(void)
{
}
