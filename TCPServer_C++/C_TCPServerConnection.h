#pragma once

#include "Poco/Net/TCPServerConnection.h"

#include "C_DBService.h"
#include "C_NetCommander.h"

class C_TCPServerConnection : public Poco::Net::TCPServerConnection
{
public:
	C_TCPServerConnection(const Poco::Net::StreamSocket& skt);
	virtual ~C_TCPServerConnection(void);

public:
	void run();

protected:
	C_DBService m_DBService;
	C_NetCommander m_NetCmder;
};

