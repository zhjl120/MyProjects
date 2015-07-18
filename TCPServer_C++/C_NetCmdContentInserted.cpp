#include "stdafx.h"
#include "C_NetCmdContentInserted.h"
#include "C_ClientMac2Socket.h"

C_NetCmdContentInserted::C_NetCmdContentInserted()
{
}

C_NetCmdContentInserted::~C_NetCmdContentInserted( void )
{
}

BOOL C_NetCmdContentInserted::HandleRequest( Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, 
	C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE;

	tstring szResourceType, szResourceId, szStartTime, szEndTime;
	StringVector vecAreaId, vecMacList;

	szResourceType = m_NetCmdDecomposer.GetCmdPara(_T("resource type"));
	szResourceId = m_NetCmdDecomposer.GetCmdPara(_T("resourece id"));
	szStartTime = m_NetCmdDecomposer.GetCmdPara(_T("start date"));
	szEndTime = m_NetCmdDecomposer.GetCmdPara(_T("end date"));

	int nSize = m_NetCmdDecomposer.GetArraySize(_T("area id"));

	for (DWORD dwCount = 0; dwCount < nSize; ++dwCount)
		vecAreaId.push_back(m_NetCmdDecomposer.GetValueByIndex(_T("area id"), dwCount));

	tstring szUrl = DBOperate.GetUrl(szResourceType.c_str(), szResourceId.c_str());

	if (szUrl.compare(_T("")) == 0)
		return FALSE;

	for (StringVector::iterator it = vecAreaId.begin(); it != vecAreaId.end(); ++it)
		DBOperate.GetMacListByAreaid(vecMacList, (*it).c_str());

	m_NetCmdComposer.SetCmdName(_T("content inserted"));
	m_NetCmdComposer.SetCmdType(_T("request"));
	m_NetCmdComposer.AddParameters(_T("url"), szUrl.c_str());
	m_NetCmdComposer.AddParameters(_T("start date"), szStartTime.c_str());
	m_NetCmdComposer.AddParameters(_T("end date"), szEndTime.c_str());
	m_NetCmdComposer.AppendPara2Root();

	tstring szResponseCmd = m_NetCmdComposer.Compose();

	for (StringVector::iterator it = vecMacList.begin(); it != vecMacList.end(); ++it)
	{
		Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr((*it).c_str());
		if (psktClient != NULL)
			psktClient->sendBytes(szResponseCmd.c_str(),szResponseCmd.length());
	}

	vecAreaId.clear();
	vecMacList.clear();

	return TRUE;
}
