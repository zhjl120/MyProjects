#pragma  once

#include "C_DBService.h"
#include "DBDataModel.h"
 
class C_DBOperate
{
public:
	C_DBOperate();
	~C_DBOperate();

public:
	BOOL IsRegistered(LPCTSTR pszMac);  //Login module
	BOOL LogOn(LPCTSTR pszMac);
	BOOL IsUpdateStartupImage(LPCTSTR pszMac , LPCTSTR pszLastTime);
	BOOL IsUpdateVersion(LPCTSTR pszVersionNum);
	tstring GetStartupImage();
	tstring GetVersionLink();
	tstring GetUIid();
	BOOL LogOff(LPCTSTR pszMac);

	BOOL IsUpdateChannels(LPCTSTR pszLastTime); //Fetch channel module
	BOOL GetChannelList(C_DirectoryInfoSet&  channelSet);

	BOOL IsHaveDirectoryIndex(LPCTSTR pszDirectoryIndex); //Fetch content module
	BOOL IsUpdateContent(LPCTSTR pszDirectoryIndex , LPCTSTR pszLastTime);
	BOOL GetContentByDirectoryId(C_ContentSet& contentset , LPCTSTR pszDirectoryIndex);

	tstring GetUrl(LPCTSTR pszType, LPCTSTR pszId);
	BOOL GetMacListByAreaid(StringVector& vecMacList, LPCTSTR pszAreaid);

	void SetMsgConsolePrompter(I_PbMessageConsolePrompter* pMsgConsolePrompter);// Print information module
	void SetSessionPool(Poco::Data::SessionPool* pSessionPool);


	BOOL UpdateScreenshot(LPCTSTR pszScreenshotName, LPCTSTR pszMac);
	BOOL UpdateChannelContentPlayTimes(LPCTSTR pszChannelContentId);
	BOOL UpdateChannelPlayTimes(LPCTSTR pszChannelContentId);
	BOOL UpdateMaxOnline(LPCTSTR pszDate, long nOnlineNum);
	BOOL UpdateTotalOnline(LPCTSTR pszDate, long nOnlineNum);
	BOOL UpdateOnlineTime(LPCTSTR pszDate, LPCTSTR pszMaC, long nOnlineTime);
	long GetCurOnlineNum();
	long GetMaxOnline(LPCTSTR pszDate);
	long GetTotalOnline(LPCTSTR pszDate);
	long GetOnlineTime(LPCTSTR pszDate, LPCTSTR pszMac);
	tstring GetRecentLogonTime(LPCTSTR pszMac);

	tstring GetAreaidByMac(LPCTSTR pszMac);

private:
	tstring GetCurrTime();
	int CompareTime(LPCTSTR beginTime, LPCTSTR endTime);
	tstring GetTemplateId(LPCTSTR pszMac);

private:
	C_DBService m_DBService;
	tstring m_szTempletId;
};