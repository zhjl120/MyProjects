#include "StdAfx.h"
#include "C_NetCommandDecomposer.h"
#include "LibHelper/C_StringHelper.h"

#define   TYPE_JSON 0x0
#define   TYPE_DATA 0x1

C_NetCommandDecomposer::C_NetCommandDecomposer(void)
{
	m_dwMessageType = TYPE_JSON;
}

C_NetCommandDecomposer::~C_NetCommandDecomposer(void)
{
}

BOOL C_NetCommandDecomposer::Decompose(LPCTSTR pszCmd)
{
	char szType[5];
	memset(szType, 0, 5);
	memcpy(szType, pszCmd, 4);

	if (strcmp(szType,_T("DATA")) == 0)
		m_dwMessageType = TYPE_DATA;

	else if(strcmp(szType,_T("JSON")) == 0)
	{
		m_dwMessageType = TYPE_JSON;

		Json::Reader jsonReader;
		CStringA szMBCS;

		C_StringHelper::UTF8toMBCS(pszCmd+12, szMBCS);
	
		if( ! jsonReader.parse((LPCTSTR)szMBCS, m_JsonRoot))
			return FALSE;
	}

	return TRUE;
}

tstring C_NetCommandDecomposer::GetCmdName()
{
	tstring  szCmdName;

	if (m_dwMessageType == TYPE_DATA) //Screenshot or something else
			szCmdName = _T("recv screenshot"); 
	else
			szCmdName = m_JsonRoot["cmd"].asString();

	return szCmdName;
}

tstring C_NetCommandDecomposer::GetCmdType()
{
	tstring szCmdType = m_JsonRoot["type"].asString();

	return szCmdType;
}

tstring C_NetCommandDecomposer::GetCmdPara(tstring szParaName)
{
	tstring szCmdValue;
	Json::Value Para = m_JsonRoot["parameters"];
	
	//if (szParaName.compare(_T("channel index")) == 0 || szParaName.compare(_T("channel_content_id")) == 0)
	//{
	//	char szIndex[10];
	//	_itoa_s(Para[szParaName.c_str()].asUInt(), szIndex, 10, 10);
	//	szCmdValue = szIndex;
	//}
	//else
		szCmdValue =  Para[szParaName.c_str()].asString();
	
	return szCmdValue;
}

int C_NetCommandDecomposer::GetArraySize(LPCTSTR pszKey)
{
	Json::Value Para = m_JsonRoot["parameters"];
	Json::Value idArray = Para[pszKey];

	return idArray.size();
}

tstring C_NetCommandDecomposer::GetValueByIndex(LPCTSTR pszKey, int nIndex)
{
	tstring szCmdValue;

	Json::Value Para = m_JsonRoot["parameters"];
	Json::Value idArray = Para[pszKey];

	szCmdValue = idArray[nIndex].asString(); 

	return szCmdValue;
}