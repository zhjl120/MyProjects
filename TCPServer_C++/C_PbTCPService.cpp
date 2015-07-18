#include "stdafx.h"
#include "C_PbTCPService.h"

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "C_TCPServerConnectionFactory.h"

C_PbTCPService::C_PbTCPService(void)
{
	SetExtensionPointId(_T("I_PbTCPService"));
	SetPluginId(_T("Europa"));
}

C_PbTCPService::~C_PbTCPService(void)
{
}

STDMETHODIMP_(int) C_PbTCPService::Start( int argc, char** argv )
{
	return Poco::Util::ServerApplication::run(argc, argv);
}

int C_PbTCPService::main( const std::vector<std::string>& args )
{
	// set-up a server socket
	Poco::Net::ServerSocket svrSocket(9911);
	// set-up a TCPServer instance
	Poco::Net::TCPServer svrTCP(new C_TCPServerConnectionFactory, svrSocket);
	// start the TCPServer
	svrTCP.start();
	// wait for CTRL-C or kill
	waitForTerminationRequest();
	// Stop the TCPServer
	svrTCP.stop();

	return Application::EXIT_OK;
}
