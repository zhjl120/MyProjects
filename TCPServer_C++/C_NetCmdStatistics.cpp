#include "stdafx.h"
#include "C_NetCmdStatistics.h"
#include "LibHelper/C_TimeHelper.h"

C_NetCmdPlayStatistics::C_NetCmdPlayStatistics()
{
}

C_NetCmdPlayStatistics::~C_NetCmdPlayStatistics( void )
{
}

BOOL C_NetCmdPlayStatistics::HandleRequest(Poco::Net::StreamSocket& sktClient,
	LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE ;

	tstring szChannelContentId = m_NetCmdDecomposer.GetCmdPara(_T("channel_content_id"));

	if( ! DBOperate.UpdateChannelContentPlayTimes(szChannelContentId.c_str()))
		return FALSE;

	if(! DBOperate.UpdateChannelPlayTimes(szChannelContentId.c_str()))
		return FALSE;

	return TRUE;
}


//////////////////////Login Statistics//////////////////////////////////////////
C_NetLoginStatistics::C_NetLoginStatistics()
{
}

C_NetLoginStatistics::~C_NetLoginStatistics()
{
}

BOOL C_NetLoginStatistics::UpdateMaxOnline( C_DBOperate& DBOperate)
{
	long nCurOnlineNum = DBOperate.GetCurOnlineNum();

	if (nCurOnlineNum < 0)
		return FALSE;

	tstring szCurDate = GetCurDate();
	long nMaxOnline = DBOperate.GetMaxOnline(szCurDate.c_str());

	if(nMaxOnline < 0)
		return FALSE;

	if(nCurOnlineNum > nMaxOnline)
		DBOperate.UpdateMaxOnline(szCurDate.c_str(), nCurOnlineNum);

	return TRUE;
}

BOOL C_NetLoginStatistics::UpdateTotalOnline(C_DBOperate& DBOperate)
{
	tstring szCurDate = GetCurDate();
	long nTotalOnline = DBOperate.GetTotalOnline(szCurDate.c_str());

	nTotalOnline++;

	if(! DBOperate.UpdateTotalOnline(szCurDate.c_str(), nTotalOnline))
		return FALSE;
	
	return TRUE;
}

BOOL C_NetLoginStatistics::UpdateOnlineTime( C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	tstring szCurDate = GetCurDate();
	long nCurOnlineTime = DBOperate.GetOnlineTime(szCurDate.c_str(), pszMac);

	if (nCurOnlineTime < 0)
		return FALSE;

	tstring szRecentLogonTime = DBOperate.GetRecentLogonTime(pszMac);

	if(szRecentLogonTime.compare(_T("")) == 0)
		return FALSE; 

	long nRecentLogonTime = C_TimeHelper::TimeStrToLong(szRecentLogonTime.c_str());
	long nCurTime = C_TimeHelper::TimeStrToLong(GetCurTime().c_str());
	
	long nTimeDiff = (nCurTime - nRecentLogonTime) / 60;

	if(nCurTime < 0)
		return FALSE;

	nCurOnlineTime += nTimeDiff;

	if(! DBOperate.UpdateOnlineTime(szCurDate.c_str(), pszMac, nCurOnlineTime))
		return FALSE;

	return TRUE;
}

tstring C_NetLoginStatistics::GetCurDate()
{
	char szCurDate[20];
	memset(szCurDate, 0, 20);

	SYSTEMTIME systime ={0};
	GetLocalTime(&systime);
	sprintf(szCurDate,"%d/%02d/%02d",systime.wYear, systime.wMonth, systime.wDay);

	return szCurDate;
}

tstring C_NetLoginStatistics::GetCurTime()
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
