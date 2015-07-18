#include "stdafx.h"
#include "C_DBOperate.h"
#include "LibHelper/C_StringConvertor.h"
#include "LibHelper/C_TimeHelper.h"

#define  DEVICE_INFORMATION_TABLE  _T("device_information")
#define  EPG_AREA_MANAGE_TABLE _T("epg_area_manage")
#define  EPG_TEMPLATE_TABLE _T("epg_templet")
#define  CONTENT_SOURCE_TABLE _T("channelcontent_source")
#define  ONLINE_STATISTICS_TABLE _T("loginstatistics")
#define  ONLINE_TIME_STATISTICS_TABLE _T("onlinetimestatistics")
#define  VERSION_INFORMATION _T("versioninfo")

C_DBOperate::C_DBOperate(): m_szTempletId(_T(""))
{
}

C_DBOperate::~C_DBOperate()
{
}

////////////////////////// Login Module //////////////////////////////////////
BOOL C_DBOperate::IsRegistered( LPCTSTR pszMac )
{
	tstring szResult ;

	szResult = m_DBService.SelectValue(_T("id"), DEVICE_INFORMATION_TABLE, _T("mac"), pszMac);

	if (!szResult.compare(_T("")))
		return FALSE;
	else 
		return TRUE;
}

BOOL C_DBOperate::LogOn( LPCTSTR pszMac )
{
	if( ! m_DBService.UpdateValue(DEVICE_INFORMATION_TABLE, _T("online") , _T("1") ,_T("mac"), pszMac) )
		return FALSE;

	tstring szCurrTime = GetCurrTime();

	if( ! m_DBService.UpdateValue(DEVICE_INFORMATION_TABLE, _T("recent_online"), szCurrTime.c_str(), _T("mac"), pszMac) )
		return FALSE;

	m_szTempletId = GetTemplateId(pszMac);//Initialization template id

	if(m_szTempletId.compare(_T("")) == 0)
		return FALSE;

	return TRUE;
}

BOOL C_DBOperate::IsUpdateStartupImage( LPCTSTR pszMac,LPCTSTR pszLastTime )
{
	//tstring szResult;

	//szResult = m_DBService.SelectValue(_T("recent_online") ,DEVICE_INFORMATION_TABLE , _T("mac") , pszMac);

	//if (szResult.compare(pszLastTime) == 0) //Need compare
	//	return FALSE;
	//else
		return TRUE;
}

tstring C_DBOperate::GetStartupImage()
{
	return m_DBService.SelectValue(_T("startup_image") ,EPG_TEMPLATE_TABLE , _T("id") , m_szTempletId.c_str());
}

tstring C_DBOperate::GetUIid() 
{
	tstring szUIid = m_DBService.SelectValue(_T("ui_id"), EPG_TEMPLATE_TABLE, _T("id"), m_szTempletId.c_str());

	if(szUIid.compare(_T("")) == 0)
		cout<<"GetUIid()\n";

	return szUIid;
}

BOOL C_DBOperate::LogOff( LPCTSTR pszMac )
{
	m_DBService.UpdateValue(DEVICE_INFORMATION_TABLE , _T("online") , _T("0") ,_T("mac"), pszMac);

	tstring szCurrTime = GetCurrTime();

	if( ! m_DBService.UpdateValue(DEVICE_INFORMATION_TABLE, _T("recent_offline"), szCurrTime.c_str(), _T("mac"), pszMac) )
		return FALSE;

	return TRUE;
}

////////////////////////// Fetch Channel Module //////////////////////////////////////
BOOL C_DBOperate::IsUpdateChannels( LPCTSTR pszLastTime )
{
	return TRUE;
}

BOOL C_DBOperate::GetChannelList( C_DirectoryInfoSet& channelList)
{
	m_DBService.SelectChannel(channelList, m_szTempletId.c_str());
	return TRUE;
}

////////////////////////// Fetch Content Module //////////////////////////////////////
BOOL C_DBOperate::IsHaveDirectoryIndex( LPCTSTR pszDirectoryIndex )
{
	tstring szResult, szChannelTablleName;

	szChannelTablleName = _T("epg_channel_");
	szChannelTablleName+= m_szTempletId;

	szResult = m_DBService.SelectValue(_T("title") ,szChannelTablleName.c_str() , _T("id") , pszDirectoryIndex);

	if (szResult.compare(_T("")) == 0) 
		return FALSE;
	else
		return TRUE;
}

BOOL C_DBOperate::IsUpdateContent( LPCTSTR pszDirectoryIndex , LPCTSTR pszLastTime)
{
	tstring szResult, szChannelTablleName;

	szChannelTablleName = _T("epg_channel_");
	szChannelTablleName+= m_szTempletId;

	szResult = m_DBService.SelectValue(_T("updatetime") ,szChannelTablleName.c_str() , _T("id") , pszDirectoryIndex);

	if (!CompareTime(szResult.c_str(), pszLastTime))
		return FALSE;
	else
		return TRUE;
}

BOOL C_DBOperate::GetContentByDirectoryId(C_ContentSet& contentset , LPCTSTR pszDirectoryIndex)
{
	m_DBService.SelectConntentByParentId(m_szTempletId.c_str(), pszDirectoryIndex, contentset);
	return TRUE;
}

////////////////////////// Print Information Module //////////////////////////////////////
void C_DBOperate::SetMsgConsolePrompter( I_PbMessageConsolePrompter* pMsgConsolePrompter )
{
	m_DBService.SetMsgConsolePrompter(pMsgConsolePrompter);
}

void C_DBOperate::SetSessionPool( Poco::Data::SessionPool* pSessionPool )
{
	m_DBService.SetSessionPool(pSessionPool);
}

////////////////////////// Play Statistics Module /////////////////////////////////////////
BOOL C_DBOperate::UpdateScreenshot( LPCTSTR pszScreenshotName , LPCTSTR pszMac)
{
	return m_DBService.UpdateValue(DEVICE_INFORMATION_TABLE, _T("screenshot"), pszScreenshotName, _T("mac"), pszMac);;
}

BOOL C_DBOperate::UpdateChannelContentPlayTimes( LPCTSTR pszChannelContentId )
{

	tstring szPlayTimes = m_DBService.SelectValue(_T("playtimes"), _T("channel_content_statistics"),\
																						   _T("channel_content_id"), pszChannelContentId);

	if(szPlayTimes.compare(_T(""))  == 0)
		return FALSE;

	long nPlayTimes = atoi(szPlayTimes.c_str());

	nPlayTimes++;

	stringstream ss ;
	ss<<nPlayTimes;
	ss>>szPlayTimes;

	if( ! m_DBService.UpdateValue(_T("channel_content_statistics"), _T("playtimes"), szPlayTimes.c_str(),\
															_T("channel_content_id"), pszChannelContentId))
			return FALSE;

	return TRUE;
}

BOOL C_DBOperate::UpdateChannelPlayTimes( LPCTSTR pszChannelContentId )
{
	//tstring szChannelId = m_DBService.SelectValue(_T("channelID"), CHANNEL_CONTENT_TABLE, _T("id"), pszChannelContentId);

	//if (szChannelId.compare(_T("")) == 0)
	//	return FALSE;

	//tstring szPlayTimes = m_DBService.SelectValue(_T("playtimes"), _T("channel_statistics"), _T("channelId"), szChannelId.c_str());

	//if (szPlayTimes.compare(_T("")) == 0)
	//	return FALSE;

	//long nPlayTimes = atoi(szPlayTimes.c_str());

	//nPlayTimes++;

	//stringstream ss ;
	//ss<<nPlayTimes;
	//ss>>szPlayTimes;

	//if( ! m_DBService.UpdateValue(_T("channel_statistics"), _T("playtimes"), szPlayTimes.c_str(),\
	//	_T("channelId"), szChannelId))
	//	return FALSE;

	return TRUE;
}

////////////////////////// Online Statistics Module //////////////////////////////////////
long C_DBOperate::GetCurOnlineNum()
{
	return m_DBService.SelectCount(DEVICE_INFORMATION_TABLE, _T("online"), _T("1"));
}

long C_DBOperate::GetMaxOnline( LPCTSTR pszDate )
{
	tstring szDate = m_DBService.SelectValue(_T("Date"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);

	if(szDate.compare(_T("")) == 0)
	{
		if( ! m_DBService.CreateOnlineStatisticsRecord(pszDate))
			return -1;//Error

		return 0;//MaxOnline
	}
	else
	{
		tstring szMaxOnline = m_DBService.SelectValue(_T("MaxOnline"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);
		if(szMaxOnline.compare(_T("")) == 0)
			return -1;//Error
		else
			return atoi(szMaxOnline.c_str());
	}
}

BOOL C_DBOperate::UpdateMaxOnline( LPCTSTR pszDate, long nOnlineNum )
{
	tstring szDate = m_DBService.SelectValue(_T("Date"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);

	if(szDate.compare(_T("")) == 0)
		if( ! m_DBService.CreateOnlineStatisticsRecord(pszDate))
			return FALSE;

	char szOnlineNum[10];
	memset(szOnlineNum,0 ,10);
	itoa(nOnlineNum,szOnlineNum,10);

	if(! m_DBService.UpdateValue(ONLINE_STATISTICS_TABLE, "MaxOnline", szOnlineNum, _T("Date"), pszDate))
		return FALSE;

	return TRUE;
}

long C_DBOperate::GetTotalOnline( LPCTSTR pszDate )
{
	tstring szDate = m_DBService.SelectValue(_T("Date"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);

	if(szDate.compare(_T("")) == 0)
	{
		if( ! m_DBService.CreateOnlineStatisticsRecord(pszDate))
			return -1;//Error

		return 0;//MaxOnline
	}
	else
	{
		tstring szTotalOnline = m_DBService.SelectValue(_T("TotalOnline"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);
		if(szTotalOnline.compare(_T("")) == 0)
			return -1;//Error
		else
			return atoi(szTotalOnline.c_str());
	}
}

BOOL C_DBOperate::UpdateTotalOnline( LPCTSTR pszDate, long nOnlineNum )
{
	tstring szDate = m_DBService.SelectValue(_T("Date"), ONLINE_STATISTICS_TABLE, _T("Date"), pszDate);

	if(szDate.compare(_T("")) == 0)
		if( ! m_DBService.CreateOnlineStatisticsRecord(pszDate))
			return FALSE;

	char szOnlineNum[10];
	memset(szOnlineNum,0 ,10);
	itoa(nOnlineNum,szOnlineNum,10);

	if(! m_DBService.UpdateValue(ONLINE_STATISTICS_TABLE, "TotalOnline", szOnlineNum, _T("Date"), pszDate))
		return FALSE;

	return TRUE;
}

long C_DBOperate::GetOnlineTime( LPCTSTR pszDate, LPCTSTR pszMac )
{
	tstring szOnlineTime = m_DBService.SelectValueEx(_T("OnlineTime"), ONLINE_TIME_STATISTICS_TABLE,
																							     _T("Date"), pszDate, _T("Mac"), pszMac);

	if(szOnlineTime.compare(_T("")) == 0)
	{
		if( ! m_DBService.CreateOnlineTimeRecord(pszDate, pszMac))
			return -1;
		
		return 0;
	}

	return atoi(szOnlineTime.c_str());
}

BOOL C_DBOperate::UpdateOnlineTime( LPCTSTR pszDate, LPCTSTR pszMac, long nOnlineTime )
{
	char szOnlineTime[20];
	memset(szOnlineTime,0 ,20);
	itoa(nOnlineTime,szOnlineTime,10);

	if(! m_DBService.UpdateValueEx(ONLINE_TIME_STATISTICS_TABLE, "OnlineTime", szOnlineTime, _T("Date"), pszDate,_T("Mac"), pszMac))
		return FALSE;

	return TRUE;
}

///////////////////////////// Other ////////////////////////////////////////////////////////////
tstring C_DBOperate::GetTemplateId( LPCTSTR pszMac )
{
	tstring szAreaid, szTempletid;

	szAreaid = m_DBService.SelectValue(_T("area_id"), DEVICE_INFORMATION_TABLE, _T("mac"), pszMac);

	if(szAreaid.compare(_T("")) == 0)
	{
		cout<<"GetTempletId(1)\n";
		return _T("");
	}

	szTempletid = m_DBService.SelectValue(_T("epg_templet_id"), EPG_AREA_MANAGE_TABLE, _T("area_id"), szAreaid.c_str());

	if(szAreaid.compare(_T("")) == 0)
	{
		cout<<"GetTempletId\n";
		return _T("");
	}

	return szTempletid;
}

tstring C_DBOperate::GetUrl( LPCTSTR pszType, LPCTSTR pszId )
{
	return m_DBService.SelectValueEx(_T("urls"), CONTENT_SOURCE_TABLE, _T("id"), pszId, _T("type"), pszType);
}

BOOL C_DBOperate::GetMacListByAreaid(StringVector& vecMacList, LPCTSTR pszAreaid )
{
	return m_DBService.SelectMacList(vecMacList, pszAreaid);
}

tstring C_DBOperate::GetRecentLogonTime(LPCTSTR pszMac)
{
	return m_DBService.SelectValue(_T("recent_online"), DEVICE_INFORMATION_TABLE, _T("mac"), pszMac);
}

////////////////////////////////Time Operate////////////////////////////////////////////////////
tstring C_DBOperate::GetCurrTime()
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

int C_DBOperate::CompareTime( LPCTSTR beginTime, LPCTSTR endTime )
{
	C_StringConvertor convertor;
	convertor.SetUnicodeString(beginTime);
	LPCSTR lpBeginTime= convertor.GetMultiByte();

	C_StringConvertor convertor2;
	convertor2.SetUnicodeString(endTime);
	LPCSTR lpEndTime = convertor2.GetMultiByte();

	time_t lBeginTiem = C_TimeHelper::TimeStrToLong((CHAR*)lpBeginTime);
	time_t lEndTime = C_TimeHelper::TimeStrToLong((CHAR*)lpEndTime);

	if(lBeginTiem > lEndTime)
		return 1;
	else if(lBeginTiem == lEndTime)
		return 0;
	else
		return -1;
}

tstring C_DBOperate::GetAreaidByMac( LPCTSTR pszMac )
{
	return m_DBService.SelectValue(_T("area_id"), DEVICE_INFORMATION_TABLE, _T("mac"), pszMac);
}

BOOL C_DBOperate::IsUpdateVersion( LPCTSTR pszVersionNum )
{
	tstring szVersion = m_DBService.SelectValue(_T("version"), VERSION_INFORMATION);

	if (szVersion.compare(_T("")) == 0)
		return FALSE;

	if(szVersion.compare(pszVersionNum) == 0)
		return FALSE;

	return TRUE;
}

tstring C_DBOperate::GetVersionLink()
{
	return  m_DBService.SelectValue(_T("apklink"), VERSION_INFORMATION );
}





