#pragma once

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "C_TCPServerConnection.h"

class C_TCPServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
	C_TCPServerConnectionFactory(void);
	virtual ~C_TCPServerConnectionFactory(void);

public:
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);
};

