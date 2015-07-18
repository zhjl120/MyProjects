#include "stdafx.h"
#include "DexterModule.h"

HANDLE  GetProcessHandle()
{
	HWND hWindowHwnd;
	DWORD dwProcessID;
	HANDLE hProcessHandle;

	hWindowHwnd = ::FindWindow(NULL, szWindowCaption);//获取窗口句柄
	::GetWindowThreadProcessId(hWindowHwnd, &dwProcessID);//通过窗口句柄获取进程ID
	hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);//通过进程ID打开进程

	int err = GetLastError();

	return hProcessHandle;
}

PDWORD  GetProcessMemory(LPVOID pvAddress,  LPVOID pvBufferLocal , DWORD dwSize)
{
	HANDLE hProcessHandle;
	PDWORD pdwReadNum = NULL;

	hProcessHandle = GetProcessHandle();
	ReadProcessMemory(hProcessHandle,pvAddress, pvBufferLocal, dwSize, pdwReadNum);//读取进程数据

	return pdwReadNum;
}
