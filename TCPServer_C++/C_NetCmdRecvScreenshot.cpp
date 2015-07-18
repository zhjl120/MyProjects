#include "stdafx.h"
#include "C_NetCmdRecvScreenshot.h"
#include "C_ClientMac2Socket.h"

C_NetCmdRecvScreenshot::C_NetCmdRecvScreenshot()
{
}

C_NetCmdRecvScreenshot::~C_NetCmdRecvScreenshot( void )
{
}

BOOL C_NetCmdRecvScreenshot::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream,
	C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	tstring szMac = pszMac;
	tstring szFileName;
	tstring szScreenshotSavePath;

	long nLength = GetLength(pszCmdStream);

	if (nLength < 1)
		return FALSE;
	
	char* pszScreenshot =new char[nLength];

	memcpy(pszScreenshot, pszCmdStream+12, nLength);

	C_ClientMac2Socket::GetScreenshotSavePath(szScreenshotSavePath);

	if (szScreenshotSavePath.compare(_T("")) == 0 || szMac.compare(_T("")) == 0)
	{
		delete pszScreenshot;
		pszScreenshot = NULL;
		return FALSE;
	}

	szScreenshotSavePath += "\\";
	szFileName+=szMac;
	szFileName+="_";
	szFileName+=GetTime();
	szFileName+=".jpg";
	szScreenshotSavePath += szFileName;

	ofstream f_out;

	f_out.open(szScreenshotSavePath.c_str(), ios::out | ios::binary);
	if (! f_out.is_open())
	{
		std::cout<<"Cannot Open image "<<std::endl;	
		return FALSE;
	}

	 f_out.write(pszScreenshot,nLength);
	 
	 f_out.close();

	DBOperate.UpdateScreenshot(szFileName.c_str(), pszMac);

	 delete pszScreenshot;
	 pszScreenshot = NULL;

	return TRUE;
}

long C_NetCmdRecvScreenshot::GetLength( LPCTSTR pszCmd )
{
	char szLength[9];

	memcpy(szLength, pszCmd+4, 8);

	szLength[8]='\0';

	long nLength = atoi(szLength);

	return nLength;
}

tstring C_NetCmdRecvScreenshot::GetTime()
{
	time_t timep;
	struct tm* pTime;

	time(&timep);

	pTime=localtime(&timep);
	timep = mktime(pTime);

	char szTime[20];
	memset(szTime, 0, 20);

	sprintf_s(szTime, "%d", timep);

	return szTime;
}

