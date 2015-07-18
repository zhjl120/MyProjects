#include "stdafx.h"
#include "C_ErrorLog.h"

LogList* C_ErrorLog::m_queLogList = new LogList;
Poco::Mutex* C_ErrorLog::m_Mutex = new Poco::Mutex;


C_ErrorLog::C_ErrorLog()
{
}

C_ErrorLog::~C_ErrorLog()
{
	delete m_Mutex;
	m_Mutex = NULL;

	delete m_queLogList;
	m_queLogList = NULL;
}

void C_ErrorLog::WriteErrLog( LPCTSTR pszErrStr )
{
	m_Mutex->lock();
	m_queLogList->push(pszErrStr);
	m_Mutex->unlock();
}

void C_ErrorLog::WriteErrLog2File(ofstream& os)
{
	if (m_queLogList->empty())
		return;

	if(os)
	{
		tstring szLog = m_queLogList->front();
		m_queLogList->pop();
		os<<GetCurrTime()<<_T(" -- ")<<szLog<<endl;
	}
}

tstring C_ErrorLog::GetCurrTime()
{
	char szCurrentDateTime[32];     
	time_t nowtime;     
	struct tm* ptm;     
	time(&nowtime);     
	ptm = localtime(&nowtime);     
	sprintf(szCurrentDateTime, "%4d/%.2d/%.2d %.2d:%.2d:%.2d",     
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,     
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 

	return szCurrentDateTime;
}