#include "StdAfx.h"
#include "C_NetCmdLogOnResponse.h"
#include "C_NetCmdLogOn.h"

C_NetCmdLogOnResponse::C_NetCmdLogOnResponse(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType = _T("response");
}

C_NetCmdLogOnResponse::~C_NetCmdLogOnResponse(void)
{
}

BOOL C_NetCmdLogOnResponse::HandleResponse( C_NetCommand* pRequestCmd , C_DBOperate& DBOperate)
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(m_szCmdType.c_str());

	C_NetCmdLogOn* pLogOnCmd = dynamic_cast<C_NetCmdLogOn*>(pRequestCmd);
	assert(pLogOnCmd);

	tstring szMac = pLogOnCmd->GetID();
	tstring szHandleReqResult = pLogOnCmd->GetResult();
	LogOnContent logonContent;

	if (szHandleReqResult.compare(_T("success")) == 0)
	{
		logonContent.m_szImagePath =  DBOperate.GetImagePath(szMac.c_str());
		//m_szResult = _T("success");
		m_NetCmdComposer.SetResult( _T("success"));
	}

	else if (szHandleReqResult.compare(_T("noupdate")) == 0)
	{
		//m_szResult = _T("noupdate");
		m_NetCmdComposer.SetResult( _T("noupdate"));
	}

	else if (szHandleReqResult.compare(_T("fail")) == 0)
	{
		//m_szResult =  _T("fail");
		m_NetCmdComposer.SetResult( _T("fail"));
	}

	m_NetCmdComposer.AddContent(logonContent);
	m_szCmdStream = m_NetCmdComposer.Compose();

	return TRUE;
}
