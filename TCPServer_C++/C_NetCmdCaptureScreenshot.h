#pragma once

#include "C_NetCommand.h"
#include "Poco/Net/Socket.h"
#include "Poco/Timespan.h"
#include <vector>

class C_NetCmdCaptureScreenshot : public C_NetCommand
{
	typedef vector< Poco::Net::Socket> SocketList;

public:
	C_NetCmdCaptureScreenshot();
	virtual ~C_NetCmdCaptureScreenshot(void);

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};