#pragma once

#include "C_NetCommand.h"

class C_NetCmdRecvScreenshot : public C_NetCommand
{
public:
	C_NetCmdRecvScreenshot();
	virtual ~C_NetCmdRecvScreenshot(void);

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);

private:
	long GetLength(LPCTSTR pszCmd);
	tstring GetTime();
};