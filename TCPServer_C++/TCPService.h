// TCPService.h : main header file for the TCPService DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "C_TCPServiceBundle.h"


// CTCPServiceApp
// See TCPService.cpp for the implementation of this class
//

class CTCPServiceApp : public CWinApp
{
public:
	CTCPServiceApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:
	C_TCPServiceBundle* m_pBundle;
};
