#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannelContentResponse :	public C_NetCommand
{
public:
	C_NetCmdFetchChannelContentResponse(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannelContentResponse(void);
};

