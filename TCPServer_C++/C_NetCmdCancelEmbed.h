#pragma once

#include "C_NetCommand.h"

class C_NetCmdCancelEmbed : public C_NetCommand
{
public:
	C_NetCmdCancelEmbed();
	~C_NetCmdCancelEmbed();

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);

private:
	tstring GetCurrTime();
	long CompareTime( LPCTSTR beginTime, LPCTSTR endTime );
};