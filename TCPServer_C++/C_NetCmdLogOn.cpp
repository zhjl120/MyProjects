#include "StdAfx.h"
#include "C_NetCmdLogOn.h"
#include "C_ClientMac2Socket.h"
#include "C_NetCmdStatistics.h"

C_NetCmdLogOn::C_NetCmdLogOn(): m_bUpdateStartImage(TRUE)
{
	m_szCmdName = _T("log on");
}

C_NetCmdLogOn::~C_NetCmdLogOn(void)
{
}

BOOL C_NetCmdLogOn::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if(! m_NetCmdDecomposer.Decompose(pszCmdStream) )
		return FALSE;

	if (DBOperate.IsRegistered(pszMac) && DBOperate.LogOn(pszMac))
	{
		m_szResult = _T("success");

		tstring szRecentlyReq = m_NetCmdDecomposer.GetCmdPara(_T("recently request"));
		tstring szVersion = m_NetCmdDecomposer.GetCmdPara(_T("version"));

		m_bUpdateStartImage = DBOperate.IsUpdateStartupImage(pszMac, szRecentlyReq.c_str());
		m_bUpdateVersion = DBOperate.IsUpdateVersion(szVersion.c_str());

		C_ClientMac2Socket::GetInstance()->AddClient(pszMac, &sktClient); 
		C_NetLoginStatistics::UpdateMaxOnline(DBOperate);
		C_NetLoginStatistics::UpdateTotalOnline(DBOperate);

		HandleResponse(sktClient, DBOperate, pszMac);
		return TRUE;
	}
	else
	{
		m_szResult =_T("fail");
		HandleResponse(sktClient, DBOperate, pszMac);
		return FALSE;
	}
}

BOOL C_NetCmdLogOn:: HandleResponse(Poco::Net::StreamSocket& sktClient, C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(_T("response"));

	LogOnInfo logOnInfo;

	if (m_szResult.compare(_T("success")) == 0)
	{
		m_NetCmdComposer.SetResult( _T("success"));

		logOnInfo.m_szUIid = DBOperate.GetUIid();

		if(m_bUpdateStartImage)
			logOnInfo.m_szImagePath =  DBOperate.GetStartupImage();
		else
			logOnInfo.m_szImagePath = _T("");//NoUpdate 

		if(m_bUpdateVersion)
			logOnInfo.m_szVersionLink = DBOperate.GetVersionLink();
		else
			logOnInfo.m_szVersionLink = _T("");
	}
	else if (m_szResult.compare(_T("fail")) == 0)
		m_NetCmdComposer.SetResult( _T("fail"));

	m_NetCmdComposer.AddContent(logOnInfo);
	tstring szResponseCmd = m_NetCmdComposer.Compose();

	sktClient.sendBytes(szResponseCmd.c_str(), strlen(szResponseCmd.c_str()));

	return TRUE;
}
