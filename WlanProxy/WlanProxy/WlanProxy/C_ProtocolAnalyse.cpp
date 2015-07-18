#include "stdafx.h"
#include "C_ProtocolAnalyse.h"
#include "C_DataQueue.h"
#include "C_SockHelper.h"


C_ProtocolAnalyse::C_ProtocolAnalyse( void )
{
	if( (m_pFile = fopen("AnalyseResults.cap","wb"))  == NULL)
		printf("Can not open the file!\n");

	if( (m_pFilter = fopen("FilterResults.cap","wb"))  == NULL)
		printf("Can not open the file!\n");
}

C_ProtocolAnalyse::~C_ProtocolAnalyse( void )
{
	fclose(m_pFile);
	fclose(m_pFilter);
}

void C_ProtocolAnalyse::HandleNextPacket()
{
	PacketInfo packet;
	int nWriteLen;

	packet = C_DataQueue::GetInstance()->PopData();
	if(packet.pPacket == NULL)
		return;
	
	nWriteLen = fwrite(packet.pPacket, 1, packet.nLength, m_pFile);
	if(nWriteLen < packet.nLength)
		printf("Error: fwrite function error!\n");

	fflush(m_pFile);

	if ( strstr(packet.pPacket, "qq.com") != false)
	{
		fwrite(packet.pPacket, 1, packet.nLength, m_pFilter);
		fflush(m_pFilter);
	}

	delete packet.pPacket;
}
