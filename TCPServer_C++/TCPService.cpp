// TCPService.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TCPService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTCPServiceApp

BEGIN_MESSAGE_MAP(CTCPServiceApp, CWinApp)
END_MESSAGE_MAP()


// CTCPServiceApp construction

CTCPServiceApp::CTCPServiceApp():
m_pBundle(NULL)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTCPServiceApp object

CTCPServiceApp theApp;


// CTCPServiceApp initialization

BOOL CTCPServiceApp::InitInstance()
{
	CWinApp::InitInstance();

	m_pBundle = new C_TCPServiceBundle;

	return TRUE;
}

int CTCPServiceApp::ExitInstance()
{
	if (m_pBundle != NULL)
		delete m_pBundle;

	return CWinApp::ExitInstance();
}

BOOL FAR WINAPI GetServiceInstance(LPCTSTR pszServiceType, void** ppInstance)
{
	if (!EMPTY_STR(pszServiceType))
	{
		if (_tcscmp(pszServiceType, _T("I_PiBundle")) == 0)
		{
			*ppInstance = static_cast<void*>(theApp.m_pBundle);
			return TRUE;
		}
	}

	return FALSE;
}