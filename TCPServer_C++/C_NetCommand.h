#pragma once

#include "C_NetCommandDecomposer.h"
#include "C_NetCommandComposer.h"
#include "C_DBOperate.h"
#include "Poco/Net/StreamSocket.h"

class C_NetCommand
{
public:
	C_NetCommand();
	virtual ~C_NetCommand(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
	void Release();

protected:
	C_NetCommandDecomposer m_NetCmdDecomposer;
	C_NetCommandComposer m_NetCmdComposer;

	tstring m_szCmdName;
	tstring m_szCmdType;
	tstring m_szResult;
};
