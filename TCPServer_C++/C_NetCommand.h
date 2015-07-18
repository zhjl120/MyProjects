#pragma once

#include "C_NetCommandDecomposer.h"
#include "C_NetCommandComposer.h"

class C_DBService;
class C_NetCommand
{
public:
	C_NetCommand(LPCTSTR pszCmdName);
	virtual ~C_NetCommand(void);

public:
	BOOL HandleRequest(LPCTSTR pCmdStream, C_DBService& DBService);
	BOOL HandleResponse( C_NetCommand* pRequestCmd, C_DBService& DBService);

public:
	LPCTSTR GetCmdStream();
	LPCTSTR GetResult();

	void Release();

protected:
	C_NetCommandDecomposer m_NetCmdDecomposer;
	C_NetCommandComposer m_NetCmdComposer;

	tstring m_szCmdStream;
	tstring m_szResult;

	tstring m_szCmdName;
	tstring m_szCmdType;
};

