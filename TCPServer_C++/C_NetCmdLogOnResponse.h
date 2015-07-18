#pragma once

#include "C_NetCommand.h"

class C_NetCmdLogOnResponse : public C_NetCommand
{
public:
	C_NetCmdLogOnResponse(LPCTSTR pszCmdName);
	virtual ~C_NetCmdLogOnResponse(void);

public:
	BOOL HandleResponse(C_NetCommand* pRequestCmd, C_DBOperate& DBOperate);
};

