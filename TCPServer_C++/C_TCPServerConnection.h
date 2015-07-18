#pragma once

#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Data/SessionPool.h"

#include "C_DBOperate.h"
#include "C_NetCommander.h"

interface I_PbMessageConsolePrompter;
class C_TCPServerConnection : public Poco::Net::TCPServerConnection
{
public:
	C_TCPServerConnection(const Poco::Net::StreamSocket& skt, I_PbMessageConsolePrompter* pMsgConsolePrompter, \
												            Poco::Data::SessionPool* pSessionPool);
	virtual ~C_TCPServerConnection(void);

public:
	void run();

protected:
	void DoRespondCommand(LPCTSTR pszCmdName, C_NetCommand* pRequestCmd);
	BOOL recvData(Poco::Net::StreamSocket& sktClient, char** ppCmdBuffer, long& nRecvLength);
	void ReleaseBuffer(char** pszBuff);

protected:
	C_DBOperate m_DBOperate;
	C_NetCommander m_NetCmder;

private:
	Poco::Data::SessionPool* m_pSessionPool;
	I_PbMessageConsolePrompter* m_pMsgConsolePrompter;
	tstring m_szClientID;
	bool m_bLogOn;
};

