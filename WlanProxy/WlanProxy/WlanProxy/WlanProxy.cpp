// WlanProxy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "C_ArpProxy.h"


int _tmain(int argc, _TCHAR* argv[])
{

//	netsh i i show in
//	netsh interface ipv4 set neighbors 36 "192.168.0.1" "08-10-77-da-0e-7a" store=active
//	netsh i i reset

	C_ArpProxy proxy;
	proxy.Start();

	system("pause");
	return 0;
}

