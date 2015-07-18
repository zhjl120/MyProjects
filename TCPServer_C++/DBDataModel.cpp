#include "stdafx.h"
#include "DBDataModel.h"

void C_Document::SetTitle( tstring strTitle )
{
	m_strTitle = strTitle;
}

void C_Document::SetDescription( tstring strDescription )
{
	m_strDescription = strDescription;
}

void C_Document::SetResource( tstring strResource )
{
	m_strResource = strResource;
}

void C_Document::SetIndex( tstring strIndex )
{
	m_strIndex = strIndex;
}

void C_Document::SetUpdateTime( tstring strUpdateTime )
{
	m_strUpdateTime = strUpdateTime;
}

void C_Document::SetChannelId( tstring strChannelId )
{
	m_strChannelId = strChannelId;
}

void C_Document::SetStream( tstring strStream )
{
	m_strStream = strStream;
}

tstring C_Document::GetTitle()
{
	return m_strTitle;
}

tstring C_Document::GetDescription()
{
	return m_strDescription;
}

tstring C_Document::GetResource()
{
	return m_strResource;
}

tstring C_Document::GetIndex()
{
	return m_strIndex;
}

tstring C_Document::GetUpdateTime()
{
	return m_strUpdateTime;
}

tstring C_Document::GetChannelId()
{
	return m_strChannelId;
}

tstring C_Document::GetStream()
{
	return m_strStream;
}


void C_Directory::Add(C_Document* pDoc )
{
	m_ContentList.push_back(pDoc);
}

void C_Directory::SetStream( tstring strStream )
{
}

tstring C_Directory::GetStream()
{
	return _T("");
}

DocumentList* C_Directory::GetDocumentList()
{
	return &m_ContentList;
}

void C_Directory::Release()
{
	for (DocumentList::iterator it = m_ContentList.begin(); it != m_ContentList.end(); ++it)
	{
		C_Directory* pDirctory  =  dynamic_cast<C_Directory*>(*it);
		C_Content* pContent = dynamic_cast<C_Content*>(*it);

		if(pDirctory != NULL)
			pDirctory->Release();
		else if(pContent != NULL)
			pContent->Release();
	}

	delete this;
}


void C_Content::Add(C_Document* pDoc )
{
}

void C_Content::Release()
{
	delete this;
}

