#include "stdafx.h"
#include "C_NetCmdCancelEmbed.h"
#include "C_AreaEvent.h"
#include "C_ClientMac2Socket.h"

C_NetCmdCancelEmbed::C_NetCmdCancelEmbed()
{
}

C_NetCmdCancelEmbed::~C_NetCmdCancelEmbed()
{
}

BOOL C_NetCmdCancelEmbed::HandleRequest( Poco::Net::StreamSocket& sktClient, 
	LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE;

	tstring szResourceType, szStartTime, szEndTime;
	StringVector vecAreaId, vecMacList;

	szResourceType = m_NetCmdDecomposer.GetCmdPara(_T("resource type"));
	szStartTime = m_NetCmdDecomposer.GetCmdPara(_T("start date"));
//	szEndTime = m_NetCmdDecomposer.GetCmdPara(_T("end date"));

	int nSize = m_NetCmdDecomposer.GetArraySize(_T("area id"));

	for (DWORD dwCount = 0; dwCount < nSize; ++dwCount)
		vecAreaId.push_back(m_NetCmdDecomposer.GetValueByIndex(_T("area id"), dwCount));

	if(CompareTime(szStartTime.c_str(), GetCurrTime().c_str()) > 0) //Not Push 
	{
		for(StringVector::iterator it = vecAreaId.begin(); it != vecAreaId.end(); ++it)
			C_AreaEvent::GetInstance()->RemoveEvent(*it, szResourceType);

		return TRUE;
	}

	//Has been pushed
	for(StringVector::iterator it = vecAreaId.begin(); it != vecAreaId.end(); ++it)
		C_AreaEvent::GetInstance()->RemovePushedEvent(*it, szResourceType);
	
	m_NetCmdComposer.SetCmdName(_T("cancel embed"));
	m_NetCmdComposer.SetCmdType(_T("request"));
	m_NetCmdComposer.AddParameters(_T("resource type"), szResourceType.c_str());
	m_NetCmdComposer.AddParameters(_T("start date"), szStartTime.c_str());
//	m_NetCmdComposer.AddParameters(_T("end date"), szEndTime.c_str());
	m_NetCmdComposer.AppendPara2Root();
	tstring szResponseCmd = m_NetCmdComposer.Compose();

	for (StringVector::iterator it = vecAreaId.begin(); it != vecAreaId.end(); ++it)
		DBOperate.GetMacListByAreaid(vecMacList, (*it).c_str());

	for (StringVector::iterator it = vecMacList.begin(); it != vecMacList.end(); ++it)
	{
		Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr((*it).c_str());
		if (psktClient != NULL)
			psktClient->sendBytes(szResponseCmd.c_str(),szResponseCmd.length());
	}

	vecMacList.clear();
	vecAreaId.clear();

	return TRUE;
}

long C_NetCmdCancelEmbed::CompareTime( LPCTSTR beginTime, LPCTSTR endTime )
{
	C_StringConvertor convertor;
	convertor.SetUnicodeString(beginTime);
	LPCSTR lpBeginTime= convertor.GetMultiByte();

	C_StringConvertor convertor2;
	convertor2.SetUnicodeString(endTime);
	LPCSTR lpEndTime = convertor2.GetMultiByte();

	time_t lBeginTiem = C_TimeHelper::TimeStrToLong((CHAR*)lpBeginTime);
	time_t lEndTime = C_TimeHelper::TimeStrToLong((CHAR*)lpEndTime);

	return lBeginTiem - lEndTime;
}

tstring C_NetCmdCancelEmbed::GetCurrTime()
{
	char szCurrTime[32];     
	time_t nowtime;     
	struct tm* ptm;     
	time(&nowtime);     
	ptm = localtime(&nowtime);     
	sprintf(szCurrTime, "%4d/%.2d/%.2d %.2d:%.2d:%.2d",     
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,     
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 

	return szCurrTime;
}