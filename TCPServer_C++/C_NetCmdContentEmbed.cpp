#include "stdafx.h"
#include "C_NetCmdContentEmbed.h"
#include "C_ClientMac2Socket.h"
#include "C_AreaEvent.h"

C_NetCmdContentEmbed::C_NetCmdContentEmbed()
{
}

C_NetCmdContentEmbed::~C_NetCmdContentEmbed( void )
{
}

long CompareTime( LPCTSTR beginTime, LPCTSTR endTime )
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

tstring GetCurrTime()
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

BOOL C_NetCmdContentEmbed::HandleRequest( Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, 
	C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE;

	tstring szResourceType, szContent, szStartTime, szEndTime;//, szPlayTime;
	StringVector vecAreaId;//, vecMacList;

	szResourceType = m_NetCmdDecomposer.GetCmdPara(_T("resource type"));
	szContent = m_NetCmdDecomposer.GetCmdPara(_T("content"));
	szStartTime = m_NetCmdDecomposer.GetCmdPara(_T("start date"));
	szEndTime = m_NetCmdDecomposer.GetCmdPara(_T("end date"));

	if(CompareTime(szStartTime.c_str(), GetCurrTime().c_str()) < 0)
		return FALSE;

	if(CompareTime(szEndTime.c_str(), szStartTime.c_str()) < 0)
		return FALSE;

	/*C_StringConvertor convertor;
	convertor.SetUnicodeString(szStartTime.c_str());
	LPCSTR lpBeginTime = convertor.GetMultiByte();

	C_StringConvertor convertor2;
	convertor2.SetUnicodeString(szEndTime.c_str());
	LPCSTR lpEndTime = convertor2.GetMultiByte();

	time_t lBeginTiem = C_TimeHelper::TimeStrToLong((CHAR*)lpBeginTime);
	time_t lEndTime = C_TimeHelper::TimeStrToLong((CHAR*)lpEndTime);
	long nPlayTime = lEndTime - lBeginTiem;

	if(nPlayTime < 0)
		return FALSE;

	char cPlayTime[20];
	itoa(nPlayTime,cPlayTime,10);
	szPlayTime = cPlayTime;

	m_NetCmdComposer.SetCmdName(_T("content embed"));
	m_NetCmdComposer.SetCmdType(_T("request"));
	m_NetCmdComposer.AddParameters(_T("resource type"), szResourceType.c_str());
	m_NetCmdComposer.AddParameters(_T("content"), szContent.c_str());
	m_NetCmdComposer.AddParameters(_T("playtime"), szPlayTime.c_str());*/
//	m_NetCmdComposer.AddParameters(_T("start date"), szStartTime.c_str());
//	m_NetCmdComposer.AddParameters(_T("end date"), szEndTime.c_str());
//	m_NetCmdComposer.AppendPara2Root();
//	tstring szResponseCmd = m_NetCmdComposer.Compose();

	int nSize = m_NetCmdDecomposer.GetArraySize(_T("area id"));
	for (DWORD dwCount = 0; dwCount < nSize; ++dwCount)
		vecAreaId.push_back(m_NetCmdDecomposer.GetValueByIndex(_T("area id"), dwCount));

	for (StringVector::iterator it = vecAreaId.begin(); it != vecAreaId.end(); ++it)
	{
		C_AreaEvent::GetInstance()->AddEvent(*it, szResourceType, szStartTime, szEndTime, szContent);//push event to AreaEvent Obj
	//	DBOperate.GetMacListByAreaid(vecMacList, (*it).c_str());
	}

	/*for (StringVector::iterator it = vecMacList.begin(); it != vecMacList.end(); ++it)
	{
		Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr((*it).c_str());
		if (psktClient != NULL)
			psktClient->sendBytes(szResponseCmd.c_str(),szResponseCmd.length());
	}*/

	vecAreaId.clear();
	//vecMacList.clear();

	return TRUE;
}

