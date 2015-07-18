#include "stdafx.h"
#include "C_AreaEvent.h"
#include "C_ClientMac2Socket.h"

Poco::Mutex* C_AreaEvent::m_ObjMutex = new Poco::Mutex;
C_AreaEvent* C_AreaEvent::m_pInstance = NULL;

DWORD __stdcall updateEventListThread(LPVOID lpVoid)
{
	C_AreaEvent *pAreaEvent = (C_AreaEvent*)lpVoid;
	pAreaEvent->DoUpdateEventList();
	return 1;
}

C_AreaEvent::C_AreaEvent()
{
	m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, _T(""));

	DWORD dwThreadID;
	m_hUpdateThread = ::CreateThread(NULL, 0, updateEventListThread, this, 0, &dwThreadID);
}

C_AreaEvent::~C_AreaEvent()
{
	::SetEvent(m_hStopEvent);
	DWORD dwExitCode = 0;
	while(1)
	{
		::GetExitCodeThread(m_hUpdateThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(100);
	}
}

void C_AreaEvent::AddEvent( tstring szAreaid, tstring szType, tstring szStartTime, tstring szEndTime, tstring szContent )
{
	Event *pEvent = new Event;
	pEvent->szAreaId = szAreaid;
	pEvent->szType = szType;
	pEvent->szStartTime = szStartTime;
	pEvent->szEndTime = szEndTime;
	pEvent->szContent = szContent;

	m_vecEventList.push_back(pEvent);
}

void C_AreaEvent::PushEvent( Poco::Net::StreamSocket& sktClient, LPCTSTR pszMac )
{
	m_PushedVecMutex.lock();
	for (EventList::iterator it = m_vecPushedEvent.begin(); it != m_vecPushedEvent.end(); )
	{
		if(CompareTime((*it)->szEndTime.c_str() , GetCurrTime().c_str()) < 0)
			it = m_vecPushedEvent.erase(it);
		else
			++it;
	}
	m_PushedVecMutex.unlock();

	tstring szAreaid = m_DBOperate.GetAreaidByMac(pszMac);
	if(szAreaid.compare(_T("")) == 0)
		return;

	m_PushedVecMutex.lock();
	for (EventList::iterator itor = m_vecPushedEvent.begin(); itor != m_vecPushedEvent.end(); ++itor)
	{
		if((*itor)->szAreaId == szAreaid)
		{
			tstring szJson = GetJsonText(*itor);
			sktClient.sendBytes(szJson.c_str(), szJson.length());
		}
	}
	m_PushedVecMutex.unlock();
}

long C_AreaEvent::CompareTime( LPCTSTR beginTime, LPCTSTR endTime )
{
	C_StringConvertor convertor;
	convertor.SetUnicodeString(beginTime);
	LPCSTR lpBeginTime= convertor.GetMultiByte();

	C_StringConvertor convertor2;
	convertor2.SetUnicodeString(endTime);
	LPCSTR lpEndTime = convertor2.GetMultiByte();

	time_t lBeginTiem = C_TimeHelper::TimeStrToLong((CHAR*)lpBeginTime);
	time_t lEndTime = C_TimeHelper::TimeStrToLong((CHAR*)lpEndTime);

	return lBeginTiem - lEndTime;
}

tstring C_AreaEvent::GetCurrTime()
{
	char szCurrTime[32];     
	time_t nowtime;     
	struct tm* ptm;     
	time(&nowtime);     
	ptm = localtime(&nowtime);     
	sprintf(szCurrTime, "%4d/%.2d/%.2d %.2d:%.2d:%.2d",     
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,     
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 

	return szCurrTime;
}

void C_AreaEvent::DoUpdateEventList()
{
	while (::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0)
	{
		m_PushVecMutex.lock();
		for (EventList::iterator it = m_vecEventList.begin(); it != m_vecEventList.end();)
		{
			tstring szAreaid = (*it)->szAreaId;
			tstring szStartTime = (*it)->szStartTime;

			if(CompareTime(szStartTime.c_str(), GetCurrTime().c_str()) <= 0 )
			{
				tstring szJson = GetJsonText(*it);
				StringVector vecMacList;
				m_DBOperate.GetMacListByAreaid(vecMacList, szAreaid.c_str());

				for (StringVector::iterator itor = vecMacList.begin(); itor != vecMacList.end(); ++itor)
				{
					Poco::Net::StreamSocket* psktClient = C_ClientMac2Socket::GetInstance()->FindSocketByMacAddr((*itor).c_str());
					if (psktClient != NULL)
						psktClient->sendBytes(szJson.c_str(), szJson.length());
				}
				m_PushedVecMutex.lock();
				m_vecPushedEvent.push_back(*it);
				m_PushedVecMutex.unlock();

				it = m_vecEventList.erase(it);
			}
			else
				++it;
		}
		m_PushVecMutex.unlock();

		Sleep(10000);//10s
	}
}

C_AreaEvent* C_AreaEvent::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_ObjMutex->lock();

		if (m_pInstance == NULL)
			m_pInstance = new C_AreaEvent;

		m_ObjMutex->unlock();
	}

	return m_pInstance;
}

void C_AreaEvent::DestroyInstance()
{
	delete m_pInstance;
	m_pInstance = NULL;

	delete m_ObjMutex;
	m_ObjMutex = NULL;
}

void C_AreaEvent::RemoveEvent( tstring szAreaid, tstring szType )
{
	m_PushVecMutex.lock();
	for(EventList::iterator it = m_vecEventList.begin(); it != m_vecEventList.end(); )
	{
		if((*it)->szAreaId == szAreaid && (*it)->szType == szType)
			it = m_vecEventList.erase(it);
		else
			++it;
	}
	m_PushVecMutex.unlock();
}

void C_AreaEvent::SetSessionPool( Poco::Data::SessionPool* pSessionPool )
{
	m_DBOperate.SetSessionPool(pSessionPool);
}

tstring C_AreaEvent::GetJsonText( Event *pEvent )
{
	C_StringConvertor convertor;
	convertor.SetUnicodeString(GetCurrTime().c_str());
	LPCSTR lpCurTime = convertor.GetMultiByte();

	C_StringConvertor convertor2;
	convertor2.SetUnicodeString(pEvent->szEndTime.c_str());
	LPCSTR lpEndTime = convertor2.GetMultiByte();

	time_t lCurTime = C_TimeHelper::TimeStrToLong((CHAR*)lpCurTime);
	time_t lEndTime = C_TimeHelper::TimeStrToLong((CHAR*)lpEndTime);
	long nPlayTime = lEndTime - lCurTime;

	if(nPlayTime < 0)
		return _T("");

	char cPlayTime[20];
	itoa(nPlayTime,cPlayTime,10);
	tstring szPlayTime = cPlayTime;

	m_NetCmdComposer.SetCmdName(_T("content embed"));
	m_NetCmdComposer.SetCmdType(_T("request"));
	m_NetCmdComposer.AddParameters(_T("resource type"), pEvent->szType.c_str());
	m_NetCmdComposer.AddParameters(_T("content"), pEvent->szContent.c_str());
	m_NetCmdComposer.AddParameters(_T("playtime"), szPlayTime.c_str());
	m_NetCmdComposer.AppendPara2Root();
	
	return m_NetCmdComposer.Compose();
}

void C_AreaEvent::RemovePushedEvent( tstring szAreaid, tstring szType )
{
	m_PushedVecMutex.lock();
	for(EventList::iterator itor = m_vecPushedEvent.begin(); itor != m_vecPushedEvent.end(); )
	{
		if((*itor)->szAreaId == szAreaid && (*itor)->szType == szType)
			itor = m_vecPushedEvent.erase(itor);
		else
			++itor;
	}
	m_PushedVecMutex.unlock();
}
