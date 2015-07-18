#pragma  once

#include "C_NetCommand.h"

class C_NetCmdContentEmbed : public C_NetCommand
{
public:
	C_NetCmdContentEmbed();
	virtual ~C_NetCmdContentEmbed(void);

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};