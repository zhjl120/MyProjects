#pragma  once

#include "C_DBOperate.h"
#include "Poco/Net/StreamSocket.h"
#include "LibHelper/C_StringConvertor.h"
#include "LibHelper/C_TimeHelper.h"
#include "C_NetCommandComposer.h"
#include "C_DBOperate.h"

class C_AreaEvent
{
protected:
	struct Event
	{
		tstring szAreaId;
		tstring szType;
		tstring szStartTime;
		tstring szEndTime;
		tstring szContent;
	};

	typedef vector<Event*> EventList;

private:
	C_AreaEvent();
	~C_AreaEvent();

public:
	static C_AreaEvent* GetInstance();
	static void DestroyInstance();

	void AddEvent(tstring szAreaid, tstring szType, tstring szStartTime, tstring szEndTime, tstring szContent);
	void RemoveEvent(tstring szAreaid, tstring szType);
	void RemovePushedEvent(tstring szAreaid, tstring szType);
	void PushEvent(Poco::Net::StreamSocket& sktClient, LPCTSTR pszMac);
	void DoUpdateEventList();
	void SetSessionPool(Poco::Data::SessionPool* pSessionPool);

private:
	tstring GetJsonText(Event *EventInfo);
	long CompareTime( LPCTSTR beginTime, LPCTSTR endTime);
	tstring GetCurrTime();

private:
	HANDLE m_hStopEvent;
	HANDLE m_hUpdateThread;
	Poco::Mutex m_PushVecMutex;
	Poco::Mutex m_PushedVecMutex;

	static C_AreaEvent* m_pInstance;
	static Poco::Mutex* m_ObjMutex;
	
	EventList m_vecEventList; //not push 
	EventList m_vecPushedEvent;//has been pushed
	C_DBOperate m_DBOperate;
	C_NetCommandComposer m_NetCmdComposer;
};