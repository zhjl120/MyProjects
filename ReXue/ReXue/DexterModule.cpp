#include "stdafx.h"
#include "DexterModule.h"

HANDLE  GetProcessHandle()
{
	HWND hWindowHwnd;
	DWORD dwProcessID;
	HANDLE hProcessHandle;

	hWindowHwnd = ::FindWindow(NULL, szWindowCaption);//��ȡ���ھ��
	::GetWindowThreadProcessId(hWindowHwnd, &dwProcessID);//ͨ�����ھ����ȡ����ID
	hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);//ͨ������ID�򿪽���

	int err = GetLastError();

	return hProcessHandle;
}

PDWORD  GetProcessMemory(LPVOID pvAddress,  LPVOID pvBufferLocal , DWORD dwSize)
{
	HANDLE hProcessHandle;
	PDWORD pdwReadNum = NULL;

	hProcessHandle = GetProcessHandle();
	ReadProcessMemory(hProcessHandle,pvAddress, pvBufferLocal, dwSize, pdwReadNum);//��ȡ��������

	return pdwReadNum;
}
