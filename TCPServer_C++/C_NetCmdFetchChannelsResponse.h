#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannelsResponse : public C_NetCommand
{
public:
	C_NetCmdFetchChannelsResponse(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannelsResponse(void);

public:
	BOOL HandleResponse( C_NetCommand* pRequestCmd, C_DBOperate& DBOperate);
	BOOL ClearChannelSet();

private:
	C_DirectoryInfoSet m_ChannelSet;
};

