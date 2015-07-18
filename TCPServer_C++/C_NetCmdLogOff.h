#pragma once

#include "C_NetCommand.h"

class C_NetCmdLogOff : public C_NetCommand
{
public:
	C_NetCmdLogOff(LPCTSTR pszCmdName);
	virtual ~C_NetCmdLogOff(void);

public:
	BOOL HandleRequest(LPCTSTR pszCmdStream, C_DBService& DBService);
};

