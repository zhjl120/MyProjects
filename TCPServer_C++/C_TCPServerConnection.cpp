#include "StdAfx.h"
#include "C_TCPServerConnection.h"

#include "Poco/Exception.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"
#include "C_AreaEvent.h"
#include "C_NetCommand.h"
#include "C_NetCmdLogOn.h"
#include "C_NetCmdLogOff.h"

#include "MessagePrompter/I_PbMessageConsolePrompter.h"
#define RECEIVE_COMMAND_BUFFER 1024

C_TCPServerConnection::C_TCPServerConnection(const Poco::Net::StreamSocket& skt, I_PbMessageConsolePrompter* pMsgConsolePrompter,\
Poco::Data::SessionPool* pSessionPool):TCPServerConnection(skt),m_pMsgConsolePrompter(pMsgConsolePrompter),
m_pSessionPool(pSessionPool), m_bLogOn(FALSE), m_szClientID(_T(""))
{
	m_DBOperate.SetSessionPool(m_pSessionPool);
	m_DBOperate.SetMsgConsolePrompter(m_pMsgConsolePrompter);
}

C_TCPServerConnection::~C_TCPServerConnection(void)
{
}

void C_TCPServerConnection::run()
{
	assert(m_pMsgConsolePrompter);
	m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_CONNECTION_0"));

	socket().setReceiveTimeout(Poco::Timespan(90,0));

	try
	{
		char* pszCmdBuffer = NULL;

		while (true)
		{
			long nCmdLen = 0;
			recvData(socket(), &pszCmdBuffer, nCmdLen);

			if (nCmdLen == 0)
			{
				if (!m_szClientID.empty())
				{
					C_NetCmdLogOff logOff;
					logOff.HandleRequest(socket(), NULL, m_DBOperate, m_szClientID.c_str());
				}

				m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_CONNECTION_1"));
				break;
			}

			if (nCmdLen > 0)
			{
				C_NetCommandDecomposer netCmdDecomposer;
				if(!netCmdDecomposer.Decompose(pszCmdBuffer))
				{
					ReleaseBuffer(&pszCmdBuffer);
					continue;
				}

				tstring szCmdName = netCmdDecomposer.GetCmdName().c_str();
				C_NetCommand* pRequestCmd = m_NetCmder.CreateCommand(szCmdName.c_str(), netCmdDecomposer.GetCmdType().c_str());
				
				if(pRequestCmd == NULL)
				{
					ReleaseBuffer(&pszCmdBuffer);
					continue;
				}

				if(szCmdName ==_T("log on") || szCmdName ==_T("log off") || szCmdName == _T("fetch channels") || szCmdName == _T("fetch content"))
				{
					if(!m_bLogOn)
					{	
						C_NetCmdLogOn *pLogOn = dynamic_cast<C_NetCmdLogOn*>(pRequestCmd);
						if(pLogOn == NULL)
							continue;

						tstring szMac = netCmdDecomposer.GetCmdPara(_T("id"));
						if( pLogOn->HandleRequest(socket(), pszCmdBuffer, m_DBOperate, szMac.c_str()) )
						{
							m_szClientID = szMac;
							m_bLogOn = true;
							ReleaseBuffer(&pszCmdBuffer);
							pLogOn->Release();
							C_AreaEvent::GetInstance()->PushEvent(socket(), szMac.c_str());
						}
						continue;
					}
				}
				
				pRequestCmd->HandleRequest(socket(), pszCmdBuffer, m_DBOperate, m_szClientID.c_str());
				pRequestCmd->Release();
				ReleaseBuffer(&pszCmdBuffer);
			}
		}
	}
	catch (Poco::Exception& exc)
	{
		if (!m_szClientID.empty())
		{
			C_NetCmdLogOff logOff;
			logOff.HandleRequest(socket(), NULL, m_DBOperate, m_szClientID.c_str());
		}
		m_pMsgConsolePrompter->PromptMessageLineBy(_T("TCP_CONNECTION_2"), exc.what());
		return;
	}
}

BOOL C_TCPServerConnection::recvData(Poco::Net::StreamSocket& sktClient, char** ppCmdBuffer, long& nRecvLength )
{
	char szHeader[12];
	char szType[5];
	char szLength[9];
	char* pszContent = NULL;

	long nLength = 0;

	memset(szType, 0, 5);
	memset(szHeader, 0 ,12);
	memset(szLength, 0, 9);

	if( socket().receiveBytes(szHeader, 12) !=12)
		return FALSE;

	memcpy(szType, szHeader, 4);
	memcpy(szLength, szHeader+4, 8);

	if (strcmp(szType,_T("DATA")) != 0 && strcmp(szType,_T("JSON")) != 0)
		return FALSE;

	nLength = atoi(szLength);

	if (nLength <= 0)
		return FALSE;

	pszContent = new char[nLength];

	int nTotalLen =0, nlen = 0;

	while(TRUE)
	{
		char szTempBuff[RECEIVE_COMMAND_BUFFER];
		memset(szTempBuff, 0 ,RECEIVE_COMMAND_BUFFER);

		nlen = socket().receiveBytes(szTempBuff, RECEIVE_COMMAND_BUFFER);

		if(nTotalLen+nlen >nLength)
		{
			delete[] pszContent;
			return FALSE;
		}

		memcpy(pszContent+nTotalLen, szTempBuff, nlen);
		nTotalLen += nlen;

		if(nTotalLen >= nLength)
			break;
	}

	if(nTotalLen > nLength)
		return FALSE;
	
	*ppCmdBuffer = new char[nLength+12+1];
	memset(*ppCmdBuffer, 0, nLength+12+1);

	memcpy(*ppCmdBuffer, szHeader, 12);
	memcpy((*ppCmdBuffer)+12, pszContent, nLength);

	(*ppCmdBuffer)[nLength+12] = '\0';

	nRecvLength = nLength + 12;

	delete[] pszContent;
	pszContent = NULL;

	return TRUE;
}

void C_TCPServerConnection::ReleaseBuffer( char** ppszBuff )
{
	if (*ppszBuff != NULL)
	{
		delete[] (*ppszBuff);
		*ppszBuff = NULL;
	}
}

