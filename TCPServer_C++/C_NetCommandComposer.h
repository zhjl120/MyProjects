#pragma once

class C_NetCommandComposer
{
public:
	C_NetCommandComposer(void);
	virtual ~C_NetCommandComposer(void);

public:
	void SetCmdName(LPCTSTR pszCmdName);
	void SetCmdType(LPCTSTR pszCmdType);
	void SetResult(LPCTSTR pszResult);

	void AddContent(LPCTSTR pszContentName, LPCTSTR pszContentValue);

	LPCTSTR Compose();
};

