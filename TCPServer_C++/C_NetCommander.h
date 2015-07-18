#pragma once

class C_NetCommand;
class C_NetCommander
{
public:
	C_NetCommander(void);
	virtual ~C_NetCommander(void);

public:
	C_NetCommand* CreateCommand(LPCTSTR pszCmdName, LPCTSTR pszCmdType);
};

