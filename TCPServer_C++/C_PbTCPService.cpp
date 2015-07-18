#include "stdafx.h"
#define  POCO_NO_UNWINDOWS 

#include "C_PbTCPService.h"

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/ThreadPool.h"

#include "Poco/Data/MySQL/Connector.h"

#include "C_TCPServerConnectionFactory.h"
#include "C_ClientMac2Socket.h"
#include "C_ErrorLog.h"
#include "C_AreaEvent.h"

DWORD __stdcall traceServiceStatusThread(LPVOID lpVoid)
{
	C_PbTCPService::S_TraceStatus* pTraceStatus = (C_PbTCPService::S_TraceStatus*)lpVoid;
	assert(pTraceStatus);

	pTraceStatus->pTCPService->DoTraceServiceStatus(pTraceStatus->pTCPServer, pTraceStatus->pDBSessionPool);

	return 1;
}

DWORD __stdcall saveErrorLogThread(LPVOID lpVoid)
{
	C_PbTCPService* pTCPService = (C_PbTCPService*)lpVoid;
	pTCPService->DoSaveErrorLog(pTCPService);
	return 1;
}

C_PbTCPService::C_PbTCPService(void)
{
	SetExtensionPointId(_T("I_PbTCPService"));
	SetPluginId(_T("Europa"));

	m_hStopTraceEvent = ::CreateEvent(NULL, FALSE, FALSE, _T(""));

	Poco::Data::MySQL::Connector::registerConnector();
}
   
C_PbTCPService::~C_PbTCPService(void)
{
	Poco::Data::MySQL::Connector::unregisterConnector();
}

STDMETHODIMP_(void) C_PbTCPService::LoadExtraConfig(void* pExtraConfigNode)
{
	if (!pExtraConfigNode)
		return;

	MSXML2::IXMLDOMNodePtr pQueryNode = reinterpret_cast<MSXML2::IXMLDOMNode*>(pExtraConfigNode);
	if (!pQueryNode)
		return;

	MSXML2::IXMLDOMNodePtr pTCPServerSettingsNode = pQueryNode->selectSingleNode(_T("TCPServiceSettings"));
	if (pTCPServerSettingsNode != NULL)
	{
		C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pTCPServerSettingsNode, _T("Port")).c_str(), &m_nSvrPort);		
		C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pTCPServerSettingsNode, _T("MaxThreads")).c_str(), &m_nSvrMaxTreads);
		C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pTCPServerSettingsNode, _T("ThreadIdleTime")).c_str(), &m_nSvrTreadIdleTime);
		C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pTCPServerSettingsNode, _T("MaxQuened")).c_str(), &m_nSvrMaxQueued);
		
		MSXML2::IXMLDOMNodePtr pThreadPoolSettingsNode = pTCPServerSettingsNode->selectSingleNode(_T("ThreadPoolSettings"));
		if (pThreadPoolSettingsNode != NULL)
		{
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pThreadPoolSettingsNode, _T("Min")).c_str(), &m_nSvrTPMin);		
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pThreadPoolSettingsNode, _T("Max")).c_str(), &m_nSvrTPMax);
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pThreadPoolSettingsNode, _T("IdleTime")).c_str(), &m_nSvrTPIdleTime);
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pThreadPoolSettingsNode, _T("StackSize")).c_str(), &m_nSvrTPStackSize);
		}
	}

	MSXML2::IXMLDOMNodePtr pDBSettingsNode = pQueryNode->selectSingleNode(_T("DBServiceSettings"));
	if (pDBSettingsNode != NULL)
	{
		m_szDBHost = C_MSXMLHelper::GetAttributeText(pDBSettingsNode, _T("Host")).c_str();		
		m_szDBPort = C_MSXMLHelper::GetAttributeText(pDBSettingsNode, _T("Port")).c_str();
		m_szDBUser = C_MSXMLHelper::GetAttributeText(pDBSettingsNode, _T("User")).c_str();
		m_szDBPassword = C_MSXMLHelper::GetAttributeText(pDBSettingsNode, _T("Password")).c_str();
		m_szDBName = C_MSXMLHelper::GetAttributeText(pDBSettingsNode, _T("Name"));

		MSXML2::IXMLDOMNodePtr pSessionPoolSettingsNode = pDBSettingsNode->selectSingleNode(_T("SessionPoolSettings"));
		if (pSessionPoolSettingsNode != NULL)
		{
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pSessionPoolSettingsNode, _T("Min")).c_str(), &m_nSPMin);		
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pSessionPoolSettingsNode, _T("Max")).c_str(), &m_nSPMax);
			C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pSessionPoolSettingsNode, _T("IdleTime")).c_str(), &m_nSPIdleTime);
		}
	}

	MSXML2::IXMLDOMNodePtr pTSSSettingsNode = pQueryNode->selectSingleNode(_T("TraceServiceStatus"));
	if (pTSSSettingsNode != NULL)
	{
		C_RadixHelper::StrToInt(C_MSXMLHelper::GetAttributeText(pTSSSettingsNode, _T("Interval")).c_str(), &m_nTSSInterval);		
		m_nTSSInterval *= 1000;
	}

	MSXML2::IXMLDOMNodePtr pScreenshotNode = pQueryNode->selectSingleNode(_T("ScreenshotPath"));
	if (pScreenshotNode != NULL)
		m_szScreenshotPath = C_MSXMLHelper::GetAttributeText(pScreenshotNode, _T("FilePath")).c_str();
}

STDMETHODIMP_(int) C_PbTCPService::Start(int argc, char** argv)
{
	return Poco::Util::ServerApplication::run(argc, argv);
}

int C_PbTCPService::main(const std::vector<std::string>& args)
{																							
	assert(m_pMsgConsolePrompter);	

	//set screen shot save path
	C_ClientMac2Socket::SetScreenshotSavePath(m_szScreenshotPath);

	// connect to DB by DB session pool
	tstring szDBConnection = "host="+m_szDBHost+";port="+m_szDBPort+";user="+m_szDBUser+";password="+m_szDBPassword+";db="+m_szDBName;
	Poco::Data::SessionPool* pDBSessionPool = new Poco::Data::SessionPool("MySQL", szDBConnection.c_str(), m_nSPMin, m_nSPMax, m_nSPIdleTime);
	assert(pDBSessionPool);

	C_AreaEvent::GetInstance()->SetSessionPool(pDBSessionPool);

	// create server connection factory
	C_TCPServerConnectionFactory* pTCPServerConnectionFactory = new C_TCPServerConnectionFactory(m_pMsgConsolePrompter, pDBSessionPool);
	assert(pTCPServerConnectionFactory);

	// create server socket
	Poco::Net::ServerSocket svrSocket(m_nSvrPort);		

	// set server parameters
	Poco::Net::TCPServerParams* pTCPSvrParams = new Poco::Net::TCPServerParams;
	assert(pTCPSvrParams);
	pTCPSvrParams->setMaxThreads(m_nSvrMaxTreads);
	pTCPSvrParams->setMaxQueued(m_nSvrMaxQueued);
	pTCPSvrParams->setThreadIdleTime(m_nSvrTreadIdleTime);

	// create server thread pool
	Poco::ThreadPool* pSvrThreadPool = new Poco::ThreadPool(m_nSvrTPMin, m_nSvrTPMax, m_nSvrTPIdleTime, m_nSvrTPStackSize);
	assert(pSvrThreadPool);

	//PooledThread * pPt= pSvrThreadPool->getThread();
	// create server object
	Poco::Net::TCPServer svrTCP(pTCPServerConnectionFactory, *pSvrThreadPool, svrSocket, pTCPSvrParams);

	// start service
	svrTCP.start();

	m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_0"), m_nSvrPort);

	// start thread of tracing service status
	S_TraceStatus traceStatus;
	traceStatus.pTCPService = this;
	traceStatus.pTCPServer = & svrTCP;
	traceStatus.pDBSessionPool = pDBSessionPool;

	DWORD dwThreadID;
	m_hTraceThread = ::CreateThread(NULL, 0, traceServiceStatusThread, &traceStatus, 0, &dwThreadID);

	//start thread of save error info
	DWORD dwLogThreadID;
	m_hLogThread = ::CreateThread(NULL, 0, saveErrorLogThread, this, 0, &dwLogThreadID);

	// wait for pressing ESC 
	WaitForQuitRequest();

	// stop service
	svrTCP.stop();

	delete pSvrThreadPool;

	C_ClientMac2Socket::DestroyInstance();
	C_AreaEvent::DestroyInstance();

	m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_1"));
	return Application::EXIT_OK;
}

void C_PbTCPService::DoTraceServiceStatus(Poco::Net::TCPServer* pTCPServer, Poco::Data::SessionPool* pDBSessionPool)
{
	assert(m_pMsgConsolePrompter);

	while(::WaitForSingleObject(m_hStopTraceEvent, 0) != WAIT_OBJECT_0)
	{
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_BEGIN"));
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_0"), pTCPServer->maxConcurrentConnections());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_1"), pTCPServer->currentConnections());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_2"), pTCPServer->refusedConnections());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_3"), pTCPServer->queuedConnections());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_4"), pTCPServer->totalConnections());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_5"), pTCPServer->currentThreads());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_SERVICE_TRACE_END"));

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_BEGIN"));
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_0"), pDBSessionPool->capacity());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_1"), pDBSessionPool->allocated());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_2"), pDBSessionPool->available());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_3"), pDBSessionPool->idle());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_4"), pDBSessionPool->used());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_5"), pDBSessionPool->dead());
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DB_SERVICE_TRACE_END"));

		::Sleep(m_nTSSInterval);
	}
}

void C_PbTCPService::DoSaveErrorLog( C_PbTCPService* pTCPService )
{
	ofstream outfile;
	outfile.open(_T("ErrorLog.txt"), ios::out | ios::app);

	if(!outfile)
	{
		pTCPService->m_pMsgConsolePrompter->PromptMessageLineBy(_T("LOG_ERROR_0"));
		return;
	}

	while (::WaitForSingleObject(pTCPService->m_hStopTraceEvent, 0) != WAIT_OBJECT_0)
	{
		C_ErrorLog::WriteErrLog2File(outfile);
		Sleep(1000);
	}

	outfile.close();
}

void C_PbTCPService::WaitForQuitRequest()
{
	while(1) 
	{
		if (::_kbhit()) 
		{
			int nKey = getch();
			if (nKey == 0x1b)
			{
				// quit tracing service status thread first
				::SetEvent(m_hStopTraceEvent);
				DWORD dwExitCode1 = 0, dwExitCode2 = 0;
				while (1)
				{
					::GetExitCodeThread(m_hTraceThread, &dwExitCode1);
					::GetExitCodeThread(m_hTraceThread, &dwExitCode2);

					if (dwExitCode1 != STILL_ACTIVE && dwExitCode2 != STILL_ACTIVE)
						break;

					::Sleep(100);
				}

				// quit main thread 
				break;
			}
		}

		::Sleep(1000);
	}
}


