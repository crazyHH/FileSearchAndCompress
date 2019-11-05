#pragma once

#include <list>
using namespace std;

class CZipArchive;

class CDFile
{
public:
	list<LPWIN32_FIND_DATA>		m_listFile;				//列表数据
	CZipArchive*				m_pZipArchive;			//文件压缩类

	typedef	struct _FILE_SEARCH_FILTER{
		//文件时间
		PFILETIME		pftCreateBegin;
		PFILETIME		pftCreateEnd;
		PFILETIME		pftAccessBegin;
		PFILETIME		pftAccessEnd;
		PFILETIME		pftWriteBegin;
		PFILETIME		pftWriteEnd;
		//文件类型
		CArray<CString> arrayType;
	}FileSearchFilter,*pFileSearchFilter;				//文件搜索的过滤条件

public:
	CDFile(void);
	~CDFile(void);
	

	//搜索文件
	BOOL	OnSearechFile(TCHAR* szFileType,TCHAR* szSearchDir,TCHAR* szStartTime,TCHAR* szEndTime,BOOL bIsSearchAll);			//请求搜索文件

	//压缩文件
	BOOL    OnFileCompress(TCHAR* szSaveDir,int dwPkgSize,BOOL bSearch);


	//功能类
	BOOL	IsTargetFile(PWIN32_FIND_DATA pstFindData, pFileSearchFilter pFileSearch);
	VOID	OnSearchAllFile(LPCTSTR pszSearchPath, pFileSearchFilter pFileSearch);	//查找指定目录下符合条件的所有文件
	VOID	OnSearchAllDiskFile(pFileSearchFilter pFileSearch);						//查找所有目录(非系统)符合条件的所有文件
	VOID	OnClearFileListCatch();													//清理文件查找缓存

	BOOL	OnCompressAllListFile(LPCTSTR lpszFileName);							//m_listFile中的所有文件压缩到指定目录文件
	VOID	OnUncompressAllFile(LPCTSTR lpszFileName, LPCTSTR lpszPath);			//解压缩文件到指定目录
	BOOL	MakeFolderExsits(TCHAR* lpszPath);			                            //保证路径存在


	//字符串转时间格式
	time_t StringToDatetime(TCHAR *str);

};
