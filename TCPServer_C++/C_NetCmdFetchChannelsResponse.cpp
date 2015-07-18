#include "StdAfx.h"
#include "C_NetCmdFetchChannelsResponse.h"


C_NetCmdFetchChannelsResponse::C_NetCmdFetchChannelsResponse(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("response");
}


C_NetCmdFetchChannelsResponse::~C_NetCmdFetchChannelsResponse(void)
{
}
