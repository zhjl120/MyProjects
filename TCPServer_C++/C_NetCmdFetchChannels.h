#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchChannels : public C_NetCommand
{
public:
	C_NetCmdFetchChannels();
	virtual ~C_NetCmdFetchChannels(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
	
private:
	BOOL HandleResponse(Poco::Net::StreamSocket& sktClient,  C_DBOperate& DBOperate, LPCTSTR pszMac);
	void ClearChannelSet(C_DirectoryInfoSet& channelSet);
};

