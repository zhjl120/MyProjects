#pragma  once

class C_SockHelper{
public:
	C_SockHelper(void);
	~C_SockHelper();

public:
	static bool InitWinSock();
	static void CleanupWinsock();

	static bool GetLocalIP(__out char* pszIp );
	static bool GetLocalMac(__out char* pszMac);
	static void GetGatewayIP(__out char* pszIp);
	static bool GetGatewayMac(__out char* pszMac);
	static void GetBroadcastIP(__out char* pszIp);
	static void GetSubnetAdd(__out char* pszIP);

private: 
	static bool GetMacByIP(__in char* pszIP, __out char* pszMac);
};