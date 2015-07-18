#include "StdAfx.h"
#include "C_TCPServerConnectionFactory.h"

C_TCPServerConnectionFactory::C_TCPServerConnectionFactory(void)
{
}

C_TCPServerConnectionFactory::~C_TCPServerConnectionFactory(void)
{
}

Poco::Net::TCPServerConnection* C_TCPServerConnectionFactory::createConnection( const Poco::Net::StreamSocket& socket )
{
	return new C_TCPServerConnection(socket);
}
