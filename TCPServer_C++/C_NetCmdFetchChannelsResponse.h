#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannelsResponse : public C_NetCommand
{
public:
	C_NetCmdFetchChannelsResponse(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannelsResponse(void);
};

