#include "StdAfx.h"
#include "C_TCPServerConnection.h"

#include "Poco/Exception.h"
#include "C_NetCommand.h"
#include "C_NetCmdLogOff.h"

#define RECEIVE_COMMAND_BUFFER 1024

C_TCPServerConnection::C_TCPServerConnection(const Poco::Net::StreamSocket& skt):
TCPServerConnection(skt)
{
}

C_TCPServerConnection::~C_TCPServerConnection(void)
{
}

void C_TCPServerConnection::run()
{
	try
	{
		while (true)
		{
			char cmdBuffer[RECEIVE_COMMAND_BUFFER];
			memset(cmdBuffer, 0, RECEIVE_COMMAND_BUFFER);

			int nCmdLen = socket().receiveBytes(cmdBuffer, RECEIVE_COMMAND_BUFFER);
			if (nCmdLen > 0)
			{
				C_NetCommandDecomposer netCmdDecomposer;

				tstring szCmd = cmdBuffer;
				netCmdDecomposer.Decompose(szCmd.c_str());
				
				C_NetCommand* pRequestCmd = m_NetCmder.CreateCommand(netCmdDecomposer.GetCmdName(), netCmdDecomposer.GetCmdType());
				if (pRequestCmd != NULL)
				{
					C_NetCmdLogOff* pLogOffCmd = dynamic_cast<C_NetCmdLogOff*>(pRequestCmd);
					if (pLogOffCmd != NULL)
					{
						pLogOffCmd->HandleRequest(szCmd.c_str(), m_DBService);
						break;//exit connection
					}
					else
					{
						if(m_DBService.Connect())
						{
							if(pRequestCmd->HandleRequest(szCmd.c_str(), m_DBService))
							{
								C_NetCommand* pResponseCmd = m_NetCmder.CreateCommand(netCmdDecomposer.GetCmdName(), _T("response"));
								if (pResponseCmd != NULL)
								{
									if(pResponseCmd->HandleResponse(pRequestCmd, m_DBService))
									{
										LPCTSTR pszCmdStream = pResponseCmd->GetCmdStream();
										socket().sendBytes(pszCmdStream, strlen(pszCmdStream));
									}

									pResponseCmd->Release();
								}
							}
							m_DBService.Disconnect();
						}
					}

					pRequestCmd->Release();
				}
			}
		}
	}
	catch (Poco::Exception& exc)
	{
		cout << exc.what() << endl;
	}
}
