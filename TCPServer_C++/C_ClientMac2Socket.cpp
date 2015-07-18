#include "stdafx.h"
#include "C_ClientMac2Socket.h"

C_ClientMac2Socket* C_ClientMac2Socket::m_pInstance = NULL;
Poco::Mutex* C_ClientMac2Socket::m_ObjMutex = new Poco::Mutex;
Poco::Mutex* C_ClientMac2Socket::m_MapMutex = new Poco::Mutex;
tstring C_ClientMac2Socket::m_szScreenshotSavePath = _T("");

C_ClientMac2Socket::C_ClientMac2Socket()
{
}

C_ClientMac2Socket::~C_ClientMac2Socket()
{
}


C_ClientMac2Socket* C_ClientMac2Socket::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_ObjMutex->lock();

		if (m_pInstance == NULL)
			m_pInstance = new C_ClientMac2Socket();

		m_ObjMutex->unlock();
	}
	
	return m_pInstance;
}

void C_ClientMac2Socket::AddClient( const tstring& szMac, Poco::Net::StreamSocket* skClient )
{
	m_MapMutex->lock();
	m_ClientMap.insert(std::map<tstring, Poco::Net::StreamSocket*>::value_type(szMac,skClient));
	m_MapMutex->unlock();
}

void C_ClientMac2Socket::RemoveClient(const tstring& szMac )
{
	m_MapMutex->lock();
	ClientMap::iterator iterClient  = m_ClientMap.find(szMac);
	m_MapMutex->unlock();

	if (iterClient != m_ClientMap.end())
		m_ClientMap.erase(iterClient);
}

Poco::Net::StreamSocket* C_ClientMac2Socket::FindSocketByMacAddr(LPCTSTR pszMac)
{
	m_MapMutex->lock();
	ClientMap::iterator iterClient = m_ClientMap.find(pszMac);
	m_MapMutex->unlock();

	if (iterClient == m_ClientMap.end())
		return NULL;

	return iterClient->second;
}

void C_ClientMac2Socket::DestroyInstance()
{
	delete m_pInstance;
	m_pInstance = NULL;

	delete m_ObjMutex;
	m_ObjMutex = NULL;

	delete m_MapMutex;
	m_MapMutex = NULL;
}

void C_ClientMac2Socket::SetScreenshotSavePath( tstring& szPath )
{
	m_szScreenshotSavePath = szPath;
}

void C_ClientMac2Socket::GetScreenshotSavePath( tstring& szPath )
{
	szPath = m_szScreenshotSavePath;
}


