#include "StdAfx.h"
#include "C_NetCommander.h"

#include "C_NetCmdLogOn.h"
#include "C_NetCmdLogOnResponse.h"
#include "C_NetCmdLogOff.h"
#include "C_NetCmdFetchChannels.h"
#include "C_NetCmdFetchChannelsResponse.h"

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
		if (szCmdName.compare(_T("log on")) == 0)
		{
			if (szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdLogOn(pszCmdName);
			else if (szCmdType.compare(_T("response")) == 0)
				return new C_NetCmdLogOnResponse(pszCmdName);
		}
		else if (szCmdName.compare(_T("log off")))
		{
			if (szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdLogOff(pszCmdName);
		}
		else if (szCmdName.compare(_T("fetch channels")))
		{
			if (szCmdType.compare(_T("request")) == 0)
				return new C_NetCmdFetchChannels(pszCmdName);
			else if (szCmdType.compare(_T("response")) == 0)
				return new C_NetCmdFetchChannelsResponse(pszCmdName);
		}
	}

	return NULL;
}
