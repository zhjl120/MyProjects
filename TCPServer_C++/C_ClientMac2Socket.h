#pragma once

#include "Poco/Net/StreamSocket.h"
#include "Poco/Mutex.h"

class C_ClientMac2Socket
{
	typedef map<tstring,Poco::Net::StreamSocket*> ClientMap;

private:
	C_ClientMac2Socket();
	~C_ClientMac2Socket();

public:
	static C_ClientMac2Socket* GetInstance();
	static void DestroyInstance();

	static void SetScreenshotSavePath(tstring& szPath);
	static void GetScreenshotSavePath(tstring& szPath);

private:
	static C_ClientMac2Socket* m_pInstance;
	static Poco::Mutex* m_ObjMutex;
	static Poco::Mutex* m_MapMutex;
	static tstring m_szScreenshotSavePath;

public:
	void AddClient(const tstring& szMac, Poco::Net::StreamSocket* skClient);
	void RemoveClient(const tstring& szMac);

	Poco::Net::StreamSocket* FindSocketByMacAddr(LPCTSTR pszMac);

private:
	ClientMap m_ClientMap;
};