#pragma  once;
#include "pcap.h"
#include "C_ProtocolAnalyse.h"
#include "C_SendPacket.h"


class C_CapturePacket{
public:
	C_CapturePacket(void);
	~C_CapturePacket(void);

public:
	bool StartListen(char* pszIPAdd, int nPort = 0);
	void StopListen();

private:
	void StartAnalysePacket();
	static DWORD WINAPI AnalyseThread( __in LPVOID lpParameter );
	static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
	static void tcp_handler(const struct pcap_pkthdr *header, const u_char *pkt_data);
	static bool udp_handler(const struct pcap_pkthdr *header, const u_char *pkt_data);

	static void dns_handler(const struct pcap_pkthdr *header, const u_char *pkt_data);

private:
	C_ProtocolAnalyse m_Analyser;
	HANDLE m_hAnalyseThread;
	static HANDLE m_hStopEvent;

	static pcap_t *m_devHandle;
	static char m_szGatewayMac[6];
	static char m_szGatewayIP[20];
	static char m_szLocalIP[20];
	static char m_szLocalMac[6];
	static char m_szBroadcastIP[20];
	static char m_szSubnetNum[15];
};