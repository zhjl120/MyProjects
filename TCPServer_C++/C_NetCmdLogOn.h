#pragma once

#include "C_NetCommand.h"

class C_NetCmdLogOn : public C_NetCommand
{
public:
	C_NetCmdLogOn();
	virtual ~C_NetCmdLogOn(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
	
private:
	BOOL HandleResponse(Poco::Net::StreamSocket& sktClient, C_DBOperate& DBOperate, LPCTSTR pszMac);

private:
	BOOL m_bUpdateStartImage;
	BOOL m_bUpdateVersion;
};

