#pragma once

#include "json.h"

class C_NetCommandDecomposer
{
public:
	C_NetCommandDecomposer(void);
	virtual ~C_NetCommandDecomposer(void);

public:
	BOOL Decompose(LPCTSTR pszCmd);

	tstring GetCmdName();
	tstring GetCmdType();
	tstring GetCmdPara(string szParaName);

	int GetArraySize(LPCTSTR pszKey);
	tstring GetValueByIndex(LPCTSTR pszKey, int nIndex);

private:
	Json::Value m_JsonRoot;
	DWORD  m_dwMessageType;
};

