#include "StdAfx.h"
#include "C_NetCommander.h"

#include "C_NetCmdLogOn.h"
#include "C_NetCmdLogOff.h"
#include "C_NetCmdFetchChannels.h"
#include "C_NetCmdFetchContent.h"
#include "C_NetCmdCaptureScreenshot.h"
#include "C_NetCmdRecvScreenshot.h"
#include "C_NetCmdStatistics.h"
#include "C_NetCmdShutdown.h"
#include "C_NetCmdContentEmbed.h"
#include "C_NetCmdCancelEmbed.h"

C_NetCommander::C_NetCommander(void)
{
}

C_NetCommander::~C_NetCommander(void)
{
}

C_NetCommand* C_NetCommander::CreateCommand(LPCTSTR pszCmdName, LPCTSTR pszCmdType)
{
	if (pszCmdName != NULL && pszCmdType != NULL)
	{
		tstring szCmdName = pszCmdName;
		tstring szCmdType = pszCmdType;

		if (szCmdName.compare(_T("log on")) == 0 && szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdLogOn;

		else if (szCmdName.compare(_T("log off")) == 0 && 	szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdLogOff;

		else if (szCmdName.compare(_T("fetch channels")) == 0 && szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdFetchChannels;

		else if (szCmdName.compare(_T("fetch content")) == 0 && szCmdType.compare(_T("request")) == 0)														 
				return new C_NetCmdFetchContent;

		else if(szCmdName.compare(_T("capture screenshot")) == 0 && szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdCaptureScreenshot;
		
		else if (szCmdName.compare(_T("recv screenshot")) == 0)
			return new C_NetCmdRecvScreenshot;

		else if(szCmdName.compare(_T("shutdown")) == 0 && szCmdType.compare(_T("request")) == 0)
			return new C_NetCmdShutdown;

		else if(szCmdName.compare(_T("content embed")) == 0 && szCmdType.compare(_T("request")) == 0)
			return new C_NetCmdContentEmbed;

		else if(szCmdName.compare(_T("cancel embed")) == 0 && szCmdType.compare(_T("request")) == 0)
			return new C_NetCmdCancelEmbed;

		else if (szCmdName.compare(_T("statistics")) == 0 && szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdPlayStatistics;

		
	}

	return NULL;
}
