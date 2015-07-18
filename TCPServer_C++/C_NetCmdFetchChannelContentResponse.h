#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannelContentResponse :	public C_NetCommand
{
public:
	C_NetCmdFetchChannelContentResponse(LPCTSTR pszCmdName);
	virtual ~C_NetCmdFetchChannelContentResponse(void);

public:
	BOOL HandleResponse( C_NetCommand* pRequestCmd, C_DBOperate& DBOperate);

private:
	C_Directory* m_pRootDir;
};

