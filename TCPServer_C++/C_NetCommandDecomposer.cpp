#include "StdAfx.h"
#include "C_NetCommandDecomposer.h"

#include "reader.h"

C_NetCommandDecomposer::C_NetCommandDecomposer(void)
{
}


C_NetCommandDecomposer::~C_NetCommandDecomposer(void)
{
}

void C_NetCommandDecomposer::Decompose(LPCTSTR pszCmd)
{
	tstring szCmdStream = pszCmd;

	Json::Value rootValue;
	Json::Reader jsonReader;
	if(jsonReader.parse(szCmdStream, rootValue))
	{
		
	}
}

LPCTSTR C_NetCommandDecomposer::GetCmdName()
{
	return NULL;
}

LPCTSTR C_NetCommandDecomposer::GetCmdType()
{
	return NULL;
}

LPCTSTR C_NetCommandDecomposer::GetCmdPara(LPCTSTR pszParaName, LPCTSTR& pszParaValue)
{
	return NULL;
}

void C_NetCommandDecomposer::Reset()
{

}
