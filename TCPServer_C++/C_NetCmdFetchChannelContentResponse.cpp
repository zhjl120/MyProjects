#include "StdAfx.h"
#include "C_NetCmdFetchChannelContentResponse.h"
#include "C_NetCmdFetchChannelContent.h"

C_NetCmdFetchChannelContentResponse::C_NetCmdFetchChannelContentResponse(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("response");
	m_pRootDir = new C_Directory;
}

C_NetCmdFetchChannelContentResponse::~C_NetCmdFetchChannelContentResponse(void)
{
}

BOOL C_NetCmdFetchChannelContentResponse::HandleResponse( C_NetCommand* pRequestCmd, C_DBOperate& DBOperate )
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(m_szCmdType.c_str());

	C_NetCmdFetchChannelContent* pCmdFetchChannel = dynamic_cast<C_NetCmdFetchChannelContent*>(pRequestCmd);

	tstring szHandleReqResult , szChannelIndex;

	szHandleReqResult = pCmdFetchChannel->GetResult();
	szChannelIndex = pCmdFetchChannel->GetChannelIndex();

	if (szHandleReqResult.compare(_T("success")) == 0)
	{
		DBOperate.GetChannelInfoRecords(m_pRootDir ,szChannelIndex.c_str());
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
		//m_szResult =  _T("fail");;
		m_NetCmdComposer.SetResult( _T("fail"));
	}

	m_NetCmdComposer.AddContent(m_pRootDir);
	m_szCmdStream = m_NetCmdComposer.Compose();

	m_pRootDir->Release();

	return TRUE;
}

