#pragma once
#include "libnet.h"

class C_SendPacket{
public:
	C_SendPacket(void);
	~C_SendPacket(void);

public:
	void StartSendArp(char *srcIP, u_char *srcMac ,char *dstIP, u_char *dstMac, ULONG Interval);
	void StopSendArp();

	void SendArpPacket();
private:
	bool BulidArpPacket(char *srcIP, u_char *srcMac ,char *dstIP, u_char *dstMac);
	
private:
	libnet_t *m_pLibNet;	
	HANDLE m_hStopEvent;
	HANDLE m_hSendThread;
	ULONG m_nSendInterval;
};