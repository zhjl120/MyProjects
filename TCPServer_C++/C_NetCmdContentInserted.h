#pragma  once

#include "C_NetCommand.h"

class C_NetCmdContentInserted : public C_NetCommand
{
public:
	C_NetCmdContentInserted();
	virtual ~C_NetCmdContentInserted(void);

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};