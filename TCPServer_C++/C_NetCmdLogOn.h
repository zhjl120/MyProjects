#pragma once

#include "C_NetCommand.h"

class C_NetCmdLogOn : public C_NetCommand
{
public:
	C_NetCmdLogOn(LPCTSTR pszCmdName);
	virtual ~C_NetCmdLogOn(void);

public:
	BOOL HandleRequest(LPCTSTR pszCmdStream, C_DBService& DBService);

public:
	LPCTSTR GetID();
};

