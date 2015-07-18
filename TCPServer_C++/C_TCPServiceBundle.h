#pragma once

#define  POCO_NO_UNWINDOWS 

#include "PluginClient\T_BundleTemplate.h"
#include "C_PbTCPService.h"

class C_TCPServiceBundle : public T_BundleTemplate<I_PiBundle>
{
public:
	C_TCPServiceBundle(void);
	virtual ~C_TCPServiceBundle(void);

protected:
	C_PbTCPService m_TCPService;

protected:
	BEGIN_REGISTER_PLUGIN()
 		REGISTER_PLUGIN(&m_TCPService)
// 		REGISTER_PLUGIN(&)
	END_REGISTER_PLUGIN()
};

