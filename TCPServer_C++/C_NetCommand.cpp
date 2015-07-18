#include "StdAfx.h"
#include "C_NetCommand.h"

C_NetCommand::C_NetCommand(LPCTSTR pszCmdName):
m_szCmdName(pszCmdName),
m_szCmdType(_T(""))
{
}

C_NetCommand::~C_NetCommand(void)
{
}

BOOL C_NetCommand::HandleRequest(LPCTSTR pCmdStream, C_DBService& DBService)
{
	return FALSE;
}

BOOL C_NetCommand::HandleResponse(C_NetCommand* pRequestCmd, C_DBService& DBService)
{
	return FALSE;
}

LPCTSTR C_NetCommand::GetCmdStream()
{
	return m_szCmdStream.c_str();
}

LPCTSTR C_NetCommand::GetResult()
{
	return m_szResult.c_str();
}

void C_NetCommand::Release()
{
	delete this;
}	
