#pragma once

#include <conio.h>
#include "PluginClient/T_PluginTemplate.h"
#include "TCPService/I_PbTCPService.h"

#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Data/SessionPool.h"

#include "LibHelper/C_MSXMLHelper.h"
#include "LibHelper/C_RadixHelper.h"

#include "MessagePrompter/I_PbMessageConsolePrompter.h"
#define POCO_NO_UNWINDOWS

class C_PbTCPService : public T_PluginTemplate<I_PbTCPService>,
					   public Poco::Util::ServerApplication
{
public:
	struct S_TraceStatus
	{
		C_PbTCPService* pTCPService;
		Poco::Net::TCPServer* pTCPServer;
		Poco::Data::SessionPool* pDBSessionPool;
	};

public:
	C_PbTCPService(void);
	virtual ~C_PbTCPService(void);

public:
	STDMETHODIMP_(void) LoadExtraConfig(void* pExtraConfigNode);
	STDMETHODIMP_(int) Start(int argc, char** argv);

public:
	int main(const std::vector<std::string>& args);
	void DoTraceServiceStatus(Poco::Net::TCPServer* pTCPServer, Poco::Data::SessionPool* pDBSessionPool);
	void DoSaveErrorLog(C_PbTCPService* pTCPService);

protected:
	void WaitForQuitRequest();

protected:
	HANDLE m_hStopTraceEvent;
	HANDLE m_hTraceThread;
	HANDLE m_hLogThread;

protected:
	//TCP server settings
	int m_nSvrPort;
	int m_nSvrMaxTreads;
	int m_nSvrTreadIdleTime;
	int m_nSvrMaxQueued;
	
	//TCP thread pool settings
	int m_nSvrTPMin;
	int m_nSvrTPMax;
	int m_nSvrTPIdleTime;
	int m_nSvrTPStackSize;

	//DB settings
	tstring m_szDBHost;
	tstring m_szDBPort;
	tstring m_szDBUser;
	tstring m_szDBPassword;
	tstring m_szDBName;

	//DB sessions pool settings
	int m_nSPMin;
	int m_nSPMax;
	int m_nSPIdleTime;

	// the interval of tracing service status
	int m_nTSSInterval;

	tstring m_szScreenshotPath;

protected:
	I_PbMessageConsolePrompter* m_pMsgConsolePrompter;

protected:
	BEGIN_DYNAMIC_CAST_PLUGIN()																				
		DYNAMIC_CAST_PLUGIN(I_PbMessageConsolePrompter, m_pMsgConsolePrompter, _T("I_PbMessageConsolePrompter"), _T("Europa"))
	END_DYNAMIC_CAST_PLUGIN()
};

