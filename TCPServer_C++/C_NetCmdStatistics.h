#pragma  once

#include "C_NetCommand.h"

class C_NetCmdPlayStatistics : public C_NetCommand
{
public:
	C_NetCmdPlayStatistics();
	virtual ~C_NetCmdPlayStatistics(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);
};


class C_NetLoginStatistics
{
public:
	C_NetLoginStatistics();
	~C_NetLoginStatistics();

public:
	static BOOL UpdateMaxOnline( C_DBOperate& DBOperate);
	static BOOL UpdateTotalOnline(C_DBOperate& DBOperate);
	static BOOL UpdateOnlineTime(C_DBOperate& DBOperate, LPCTSTR pszMac);

private:
	static tstring GetCurDate();
	static tstring GetCurTime();
};