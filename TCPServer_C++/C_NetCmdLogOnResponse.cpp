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

BOOL C_NetCmdLogOnResponse::HandleResponse( C_NetCommand* pRequestCmd, C_DBService& DBService )
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(m_szCmdType.c_str());

	tstring szHandleReqResult = pRequestCmd->GetResult();
	if (szHandleReqResult.compare(_T("success")) == 0)
	{
		C_NetCmdLogOn* pLogOnCmd = dynamic_cast<C_NetCmdLogOn*>(pRequestCmd);
		assert(pLogOnCmd);
		LPCTSTR pszDeviceID = pLogOnCmd->GetID();

		//get startup image info from DB by device ID
		LPCTSTR pszStartupImage = NULL;
		//write DB operation code here
		//
		//

		m_NetCmdComposer.AddContent(_T("startup image"), pszStartupImage);
		m_szResult = _T("success");
	}
	else if (szHandleReqResult.compare(_T("noupdate")) == 0)
	{
		m_NetCmdComposer.AddContent(_T("startup image"), szHandleReqResult.c_str());
		m_szResult = _T("success");
	}
	else if (szHandleReqResult.compare(_T("fail")) == 0)
		m_szResult = szHandleReqResult;

	m_NetCmdComposer.SetResult(m_szResult.c_str());
	
	m_szCmdStream = m_NetCmdComposer.Compose();

	return TRUE;
}
