#pragma  once
#include <queue>
#include "Poco/Mutex.h"
#include <fstream>

typedef queue<tstring> LogList;

class C_ErrorLog
{
public:
	C_ErrorLog();
	~C_ErrorLog();

public:
	static void WriteErrLog(LPCTSTR pszErrStr);
	static void WriteErrLog2File(ofstream& os);

private:
	static tstring GetCurrTime();

private:
	static LogList* m_queLogList;
	static Poco::Mutex* m_Mutex;
};