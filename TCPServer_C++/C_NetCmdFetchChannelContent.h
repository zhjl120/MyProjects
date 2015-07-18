#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannelContent :	public C_NetCommand
{
public:
	C_NetCmdFetchChannelContent(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannelContent(void);
};

