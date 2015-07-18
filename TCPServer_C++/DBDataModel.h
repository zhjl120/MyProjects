#pragma once

struct LogOnInfo
{
	tstring m_szImagePath;
	tstring m_szUIid;
	tstring m_szVersionLink;
};

struct Content
{
	tstring m_szIndex;
	tstring m_szType;
	tstring m_szTitle;
	tstring m_szDescription;
	tstring m_szResource;
	tstring m_szUrls;
	tstring m_szDirecotryId;
	tstring m_szUpdatetime;
	tstring m_szQueue;
};

struct DirectoryInfo
{
	tstring m_szTitle;
	tstring m_szDescription;
	tstring m_szResource;
	tstring m_szIndex;
	tstring m_szUpdatetime;
};
//
//struct FileInfo
//{
//	tstring m_szIndex;
//	tstring m_szType;
//	tstring m_szTitle;
//	tstring m_szDescription;
//	tstring m_szResource;
//	tstring m_szUrls;
//	tstring m_szDirecotryId;
//};

typedef vector<Content*> C_ContentSet;
typedef vector<DirectoryInfo*> C_DirectoryInfoSet;
//typedef vector<FileInfo*> C_FileInfoSet;
typedef vector<tstring> StringVector;