#pragma once

#include "C_NetCommand.h"

class C_NetCmdLogOff : public C_NetCommand
{
public:
	C_NetCmdLogOff();
	virtual ~C_NetCmdLogOff(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};

