#include "StdAfx.h"
#include "C_TCPServerConnectionFactory.h"

#include "C_TCPServerConnection.h"

C_TCPServerConnectionFactory::C_TCPServerConnectionFactory(I_PbMessageConsolePrompter* pMsgConsolePrompter,\
Poco::Data::SessionPool* pSessionPool):
m_pMsgConsolePrompter(pMsgConsolePrompter), m_pSessionPool(pSessionPool)
{
}

C_TCPServerConnectionFactory::~C_TCPServerConnectionFactory(void)
{
}

Poco::Net::TCPServerConnection* C_TCPServerConnectionFactory::createConnection( const Poco::Net::StreamSocket& socket )
{
	return new C_TCPServerConnection(socket, m_pMsgConsolePrompter, m_pSessionPool);
}
