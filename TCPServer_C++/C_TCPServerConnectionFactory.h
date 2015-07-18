#pragma once

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Data/SessionPool.h"

interface I_PbMessageConsolePrompter;
class C_TCPServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
	C_TCPServerConnectionFactory(I_PbMessageConsolePrompter* pMsgConsolePrompter, Poco::Data::SessionPool* pSessionPool);
	virtual ~C_TCPServerConnectionFactory(void);

public:
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);

protected:
	Poco::Data::SessionPool* m_pSessionPool;
	I_PbMessageConsolePrompter* m_pMsgConsolePrompter;
};

