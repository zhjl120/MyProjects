#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannels : public C_NetCommand
{
public:
	C_NetCmdFetchChannels(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannels(void);
};

