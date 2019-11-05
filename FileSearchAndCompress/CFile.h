#pragma once

#include <list>
using namespace std;

class CZipArchive;

class CDFile
{
public:
	list<LPWIN32_FIND_DATA>		m_listFile;				//�б�����
	CZipArchive*				m_pZipArchive;			//�ļ�ѹ����

	typedef	struct _FILE_SEARCH_FILTER{
		//�ļ�ʱ��
		PFILETIME		pftCreateBegin;
		PFILETIME		pftCreateEnd;
		PFILETIME		pftAccessBegin;
		PFILETIME		pftAccessEnd;
		PFILETIME		pftWriteBegin;
		PFILETIME		pftWriteEnd;
		//�ļ�����
		CArray<CString> arrayType;
	}FileSearchFilter,*pFileSearchFilter;				//�ļ������Ĺ�������

public:
	CDFile(void);
	~CDFile(void);
	

	//�����ļ�
	BOOL	OnSearechFile(TCHAR* szFileType,TCHAR* szSearchDir,TCHAR* szStartTime,TCHAR* szEndTime,BOOL bIsSearchAll);			//���������ļ�

	//ѹ���ļ�
	BOOL    OnFileCompress(TCHAR* szSaveDir,int dwPkgSize,BOOL bSearch);


	//������
	BOOL	IsTargetFile(PWIN32_FIND_DATA pstFindData, pFileSearchFilter pFileSearch);
	VOID	OnSearchAllFile(LPCTSTR pszSearchPath, pFileSearchFilter pFileSearch);	//����ָ��Ŀ¼�·��������������ļ�
	VOID	OnSearchAllDiskFile(pFileSearchFilter pFileSearch);						//��������Ŀ¼(��ϵͳ)���������������ļ�
	VOID	OnClearFileListCatch();													//�����ļ����һ���

	BOOL	OnCompressAllListFile(LPCTSTR lpszFileName);							//m_listFile�е������ļ�ѹ����ָ��Ŀ¼�ļ�
	VOID	OnUncompressAllFile(LPCTSTR lpszFileName, LPCTSTR lpszPath);			//��ѹ���ļ���ָ��Ŀ¼
	BOOL	MakeFolderExsits(TCHAR* lpszPath);			                            //��֤·������


	//�ַ���תʱ���ʽ
	time_t StringToDatetime(TCHAR *str);

};
