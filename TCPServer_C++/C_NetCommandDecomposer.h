#pragma once

class C_NetCommandDecomposer
{
public:
	C_NetCommandDecomposer(void);
	virtual ~C_NetCommandDecomposer(void);

public:
	void Decompose(LPCTSTR pszCmd);

	LPCTSTR GetCmdName();
	LPCTSTR GetCmdType();

	LPCTSTR GetCmdPara(LPCTSTR pszParaName, LPCTSTR& pszParaValue);

	void Reset();
};

