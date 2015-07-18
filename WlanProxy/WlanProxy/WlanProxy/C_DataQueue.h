#pragma  once
#include <afxmt.h>
#include <queue>
#include <string>
using namespace std;

struct PacketInfo
{
	char* pPacket;
	long nLength;
};


class C_DataQueue{
	typedef queue<PacketInfo> DataQueue;

private:
	C_DataQueue(void);
	~C_DataQueue(void);

public:
	static C_DataQueue* GetInstance();
	static void DestoryInstance();

public:
	void AddData(PacketInfo packet);
	PacketInfo PopData();

private:
	static C_DataQueue* m_pInstance;
	static CMutex* m_pObjMutex;
	DataQueue m_DataQueue;
};