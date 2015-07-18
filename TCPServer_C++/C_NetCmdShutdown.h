#pragma  once

#include "C_NetCommand.h"
#include "Poco/Net/Socket.h"
#include <vector>

class C_NetCmdShutdown : public C_NetCommand
{
	typedef vector< Poco::Net::Socket> SocketList;

public:
	C_NetCmdShutdown();
	virtual ~C_NetCmdShutdown(void);

public:
	BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};




