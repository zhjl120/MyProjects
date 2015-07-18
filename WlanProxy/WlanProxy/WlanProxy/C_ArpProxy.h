#pragma once
#include <afxmt.h>
#include <vector>
using namespace std;


struct IPMAC{
	char szIp[16];
	byte byMac[6];
	char szDescription[30];
};

typedef vector<IPMAC> IpMacVec;

class C_ArpProxy{
public:
	C_ArpProxy(void);
	~C_ArpProxy(void);

public:
	void Start();

private:
	void GetOnlineClient();
	static DWORD WINAPI SendArpThread(__in  LPVOID lpParameter);

private:
	static  IpMacVec* m_pClientList;
	static CMutex* m_pMutex;
};