#include "StdAfx.h"
#include "C_NetCommand.h"

C_NetCommand::C_NetCommand():m_szCmdName(_T("")),m_szCmdType(_T("")),m_szResult(_T(""))
{
}

C_NetCommand::~C_NetCommand(void)
{
}

BOOL C_NetCommand::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	return FALSE;
}

void C_NetCommand::Release()
{
	delete this;
}	
