// FileConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileConfig.h"
#include "FileConfigDlg.h"

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFileConfigDlg �Ի���




CFileConfigDlg::CFileConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileConfigDlg::IDD, pParent)
	, m_strType(_T(""))
	, m_strStartTime(_T(""))
	, m_strEndTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strType);
	DDX_Text(pDX, IDC_EDIT3, m_strStartTime);
	DDX_Text(pDX, IDC_EDIT2, m_strEndTime);
}

BEGIN_MESSAGE_MAP(CFileConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CFileConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileConfigDlg ��Ϣ�������

BOOL CFileConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ʼ�������ļ�Ŀ¼
	TCHAR szConfig[MAX_PATH] = {0};

	::GetModuleFileName(NULL, szConfig, MAX_PATH);
	int nPathLen = 0;

	for (int i = 0; i != MAX_PATH; i++)
	{
		if (!szConfig[i])
			{ break; }

		if (szConfig[i] == _T('\\'))
			{ nPathLen = i; }
	}

	szConfig[nPathLen] = _T('\0');

	m_strConfig = szConfig;
	m_strConfig.Append(_T("\\FileConfig.ini"));

	//���������ļ�,���µ�������ʾ
	LoadConfig();

	SetDlgItemText(IDC_EDIT1, m_strType);
	SetDlgItemText(IDC_EDIT3, m_strStartTime);
	SetDlgItemText(IDC_EDIT2, m_strEndTime);
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFileConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CFileConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileConfigDlg::OnBnClickedOk()
{
	//�����ļ�
	this->UpdateData();
	

	CFileDialog fileDialog(	FALSE, 
							"exe", 
							"FileServer", 
							OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, 
							"Exe Files(*.exe)|*.exe"
							);
	if(IDOK != fileDialog.DoModal())
	{
		MessageBox("File dialog error!");
		return ;
	}
	
	HRSRC hFindRes = NULL;
	
	//����exe��Դ�ļ�
	hFindRes = ::FindResource(NULL, MAKEINTRESOURCE(IDR_BIN1), "BIN");
	
	if(NULL == hFindRes)
	{
		MessageBox("Find Resource error!");
		return;
	}

	HGLOBAL hLoadRes = ::LoadResource(NULL, hFindRes);
	if(NULL == hFindRes)
	{
		MessageBox("Load resource error!");
		return;	
	}

	DWORD dwResSize = ::SizeofResource(NULL, hFindRes);

	LPBYTE lpResData = (LPBYTE)::LockResource(hLoadRes);
	if(NULL == lpResData)
	{
		MessageBox("Lock resource error!");
		::FreeResource(hLoadRes);
		return;
	}
	
	LPBYTE lpData = new BYTE[dwResSize];
	if(NULL == lpData)
	{
		MessageBox("new error!");
		::FreeResource(hLoadRes);
		return;
	}
	
	memcpy(lpData, lpResData, dwResSize);
	::FreeResource(hLoadRes);

	//�ļ�����
	if(this->m_strType.GetLength() > 0)
	{
		this->OnReplaceData(lpData, 
							dwResSize,
							(LPBYTE)"!!!Type!!!", 
							strlen("!!!Type!!!"),
							(LPBYTE)this->m_strType.GetBuffer(0), 
							this->m_strType.GetLength()
							);
	}
	else
	{
		MessageBox("Replace data error!");
		return;
	}
	

	//��ʼʱ��
	if(this->m_strStartTime.GetLength() > 0)
	{
		this->OnReplaceData(lpData, 
							dwResSize,
							(LPBYTE)"!!!StartTime!!!", 
							strlen("!!!StartTime!!!"),
							(LPBYTE)this->m_strStartTime.GetBuffer(0), 
							this->m_strStartTime.GetLength()
							);
	}
	else
	{
		MessageBox("Replace data error!");
		return;
	}

	//����ʱ��
	if(this->m_strEndTime.GetLength() > 0)
	{
		this->OnReplaceData(lpData, 
							dwResSize,
							(LPBYTE)"!!!EndTime!!!", 
							strlen("!!!EndTime!!!"),
							(LPBYTE)this->m_strEndTime.GetBuffer(0), 
							this->m_strEndTime.GetLength()
							);
	}
	else
	{
		MessageBox("Replace data error!");
		return;
	}

	//���浽�����ļ�
	SaveConfig();

	//���浽�ļ�
	FILE *file = fopen(fileDialog.GetPathName().GetBuffer(0), "wb");
	if (!file)
	{
		MessageBox("Create file error!!!");
		return ;
	}

	fwrite(lpData, 1, dwResSize, file);
	fclose(file);

	//�޸��ļ�ʱ��
	OnChangeFileTime(fileDialog.GetPathName().GetBuffer(0));
}



LPBYTE CFileConfigDlg::OnFindSign(LPBYTE bData, DWORD dwDataSize, LPBYTE bFind, DWORD dwFindLength)
{
	LPBYTE lpFindPos = bData;
	if(dwDataSize < dwFindLength || dwFindLength == 0)
	{
		return NULL;
	}
	BOOL	bIsFind;
	DWORD dwIndex = 0;
	for(; dwIndex < dwDataSize - dwFindLength; dwIndex ++)
	{
		bIsFind = TRUE;

		for(DWORD dwFindIndex = 0; dwFindIndex < dwFindLength; dwFindIndex++)
		{
			if(lpFindPos[dwFindIndex] != bFind[dwFindIndex])
			{
				bIsFind = FALSE;
				break;
			}
		}
		if(bIsFind)
		{
			break;
		}
		else
		{
			lpFindPos++;
		}
	}
	if(bIsFind)
	{
		return lpFindPos;
	}
	else
	{
		return NULL;
	}
	
}
BOOL CFileConfigDlg::OnReplaceData(	LPBYTE bData, 
										DWORD dwDataSize, 
										LPBYTE bSouData,
										DWORD dwSouDataLen,
										LPBYTE bReplaceData, 
										DWORD dwReplaceLength
										)
{
	LPBYTE lpFindPos = this->OnFindSign(bData, dwDataSize, bSouData, dwSouDataLen);
	if (!lpFindPos)
	{
		return FALSE;
	}

	if (dwReplaceLength < dwSouDataLen)
	{
		for (DWORD dwIndex = 0; dwIndex < dwSouDataLen; dwIndex++)
		{
			lpFindPos[dwIndex] = 0x0;
		}
	}

	for (DWORD dwIndex = 0; dwIndex < dwReplaceLength; dwIndex++)
	{
		lpFindPos[dwIndex] = bReplaceData[dwIndex];
	}

	return TRUE;
}


void CFileConfigDlg::OnChangeFileTime(LPCTSTR lpszName)
{	
	HANDLE hFile = ::CreateFile(lpszName,
								GENERIC_READ|GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);

	if (INVALID_HANDLE_VALUE == hFile) {
		return;
	}

	SYSTEMTIME	stCreate, stAccess, stModify;
	FILETIME ftCreate, ftAccess, ftModify;

	::GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify);

	::FileTimeToSystemTime(&ftCreate, &stCreate);
	::FileTimeToSystemTime(&ftAccess, &stAccess);
	::FileTimeToSystemTime(&ftModify, &stModify);

	stCreate.wYear = 2008;
	stAccess.wYear = 2015;
	stModify.wYear = 2008;

	::SystemTimeToFileTime(&stCreate, &ftCreate);
	::SystemTimeToFileTime(&stAccess, &ftAccess);
	::SystemTimeToFileTime(&stModify, &ftModify);

	if (!::SetFileTime(hFile, &ftCreate, &ftAccess, &ftModify)) {
		::MessageBox(NULL, _T("Change file time failed!"), _T("warning"), MB_OK);
	}

	::CloseHandle(hFile);
}


void CFileConfigDlg::LoadConfig()
{
	if (!::PathFileExists(m_strConfig)) {
		return;
	}

	TCHAR* pRead = (TCHAR*)malloc(0x400);

	ZeroMemory(pRead, 0x400);
	GetPrivateProfileString(_T("Config"), _T("Type"), NULL, pRead, 0x400, m_strConfig);
	m_strType = pRead;

	ZeroMemory(pRead, 0x400);
	GetPrivateProfileString(_T("Config"), _T("StartTime"), NULL, pRead, 0x400, m_strConfig);
	m_strStartTime = pRead;

	ZeroMemory(pRead, 0x400);
	GetPrivateProfileString(_T("Config"), _T("EndTime"), NULL, pRead, 0x400, m_strConfig);
	m_strEndTime = pRead;

	free(pRead);
}

void CFileConfigDlg::SaveConfig()
{
	if (!::PathFileExists(m_strConfig)) {
		HANDLE hFile = CreateFile(m_strConfig, GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile) {
			CloseHandle(hFile);
		} else {
			return;
		}
	}

	WritePrivateProfileString(_T("Config"), _T("Type"), m_strType, m_strConfig);
	WritePrivateProfileString(_T("Config"), _T("StartTime"), m_strStartTime, m_strConfig);
	WritePrivateProfileString(_T("Config"), _T("EndTime"), m_strEndTime, m_strConfig);
}


