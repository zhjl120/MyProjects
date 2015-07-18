#pragma once

#include "PluginClient/T_PluginTemplate.h"
#include "TCPService/I_PbTCPService.h"
#include "Poco/Util/ServerApplication.h"

class C_PbTCPService : public T_PluginTemplate<I_PbTCPService>,
					   public Poco::Util::ServerApplication
{
public:
	C_PbTCPService(void);
	virtual ~C_PbTCPService(void);

public:
	STDMETHODIMP_(int) Start(int argc, char** argv);

public:
	int main(const std::vector<std::string>& args);
};

