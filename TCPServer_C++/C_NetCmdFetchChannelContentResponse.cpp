#include "StdAfx.h"
#include "C_NetCmdFetchChannelContentResponse.h"

C_NetCmdFetchChannelContentResponse::C_NetCmdFetchChannelContentResponse(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("response");
}

C_NetCmdFetchChannelContentResponse::~C_NetCmdFetchChannelContentResponse(void)
{
}
