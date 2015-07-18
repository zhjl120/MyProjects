#include "StdAfx.h"
#include "C_NetCommandComposer.h"

#include "LibHelper/C_StringHelper.h"

C_NetCommandComposer::C_NetCommandComposer(void)
{
}

C_NetCommandComposer::~C_NetCommandComposer(void)
{
}

void C_NetCommandComposer::SetCmdName(LPCTSTR pszCmdName)
{
	m_root["cmd"] = pszCmdName;
}

void C_NetCommandComposer::SetCmdType(LPCTSTR pszCmdType)
{
	m_root["type"] =pszCmdType;
}

void C_NetCommandComposer::SetResult(LPCTSTR pszResult)
{
	m_root["result"] = pszResult;
	m_szResult = pszResult;
}

void C_NetCommandComposer::AddContent(LogOnInfo& logOnInfo )
{
	if (m_szResult.compare(_T("success")) == 0)
	{
		tstring szImagePath = logOnInfo.m_szImagePath;
		tstring szUIid = logOnInfo.m_szUIid;	
		tstring szVersion = logOnInfo.m_szVersionLink;

		if(szImagePath.compare(_T("")) == 0)
			m_content["startup image"] = "null";
		else
			m_content["startup image"] = szImagePath;

		if(szUIid.compare(_T("")) == 0)
			m_content["UI id"] = "null";
		else
			m_content["UI id"] = szUIid;

		if(szVersion.compare(_T("")) == 0)
			m_content["apklink"] = "null";
		else
			m_content["apklink"] = szVersion;

		m_root["content"] = m_content;
	}
	
	else if (m_szResult.compare(_T("fail")) == 0)
		m_root["content"] = "null";

}

void C_NetCommandComposer::AddContent( C_DirectoryInfoSet& channelSet )
{
	if (m_szResult.compare(_T("success")) == 0)
	{
		if(channelSet.size() < 1)
			m_root["content"] = "null";
		else
		{
			for (C_DirectoryInfoSet::iterator it = channelSet.begin() ; it != channelSet.end() ; ++it )
			{
				m_item["title"] = (*it)->m_szTitle;
				m_item["description"] = (*it)->m_szDescription;
				m_item["resource"] = (*it)->m_szResource;
				m_item["index"] = (*it)->m_szIndex;
				m_item["updatetime"] = (*it)->m_szUpdatetime;
				
				m_content.append(m_item);
			}
			m_root["content"] = m_content;
		}
	}

	else if (m_szResult.compare(_T("noupdate")) == 0)
		m_root["content"] = "null";

	else if (m_szResult.compare(_T("fail")) == 0)
		m_root["content"] = "null";

}

void C_NetCommandComposer::AddContent(C_ContentSet& contentset, LPCTSTR pszParentId)
{
	if (m_szResult.compare(_T("success")) == 0)
	{
		Json::Value infoArray;

		if(contentset.size() < 1)
			m_content["info"] = "null";
		else
		{
			for (C_ContentSet::iterator it = contentset.begin() ; it != contentset.end() ; ++it )
			{
				m_item["title"] = (*it)->m_szTitle;
				m_item["description"] = (*it)->m_szDescription;
				m_item["resource"] = (*it)->m_szResource;
				m_item["index"] = (*it)->m_szIndex;
				m_item["updatetime"] = (*it)->m_szUpdatetime;
				m_item["urls"] = (*it)->m_szUrls;
				m_item["type"] = (*it)->m_szType;
	//			m_item["directoryid"] = (*it)->m_szDirecotryId;
				m_item["queue"] = (*it)->m_szQueue;

				infoArray.append(m_item);
			}
			m_content["info"] = infoArray;
		}
		
		/*	if(fileSet.size() < 1)
		m_content["file"] = "null";
		else
		{
		for (C_FileInfoSet::iterator it = fileSet.begin() ; it != fileSet.end() ; ++it )
		{
		m_item["title"] = (*it)->m_szTitle;
		m_item["description"] = (*it)->m_szDescription;
		m_item["resource"] = (*it)->m_szResource;
		m_item["index"] = (*it)->m_szIndex;
		m_item["type"] = (*it)->m_szType;
		m_item["urls"] = (*it)->m_szUrls;
		m_item["directoryid"] = (*it)->m_szDirecotryId;

		fileArray.append(m_item);
		}
		m_content["file"] = fileArray;
		}*/

		m_content["directoryid"] = pszParentId;
		m_root["content"] = m_content;
	}

	else if (m_szResult.compare(_T("noupdate")) == 0)
		m_root["content"] = "null";

	else if (m_szResult.compare(_T("fail")) == 0)
		m_root["content"] = "null";

}

tstring C_NetCommandComposer::Compose()
{
	Json::FastWriter writer;

	tstring szJsonText = writer.write(m_root);

	CStringA szUTF8;
	C_StringHelper::MBCStoUTF8((char*)szJsonText.c_str(), szUTF8);

	return (LPCTSTR)szUTF8;
}

void C_NetCommandComposer::AddParameters( LPCTSTR pszKey, LPCTSTR pszValue )
{
	m_Param[pszKey] = pszValue;
}

void C_NetCommandComposer::AppendPara2Root()
{
	m_root["parameters"] = m_Param;
}
