// FileConfigDlg.h : 头文件
//

#pragma once


// CFileConfigDlg 对话框
class CFileConfigDlg : public CDialog
{
// 构造
public:
	CFileConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILECONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    

	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	LPBYTE	OnFindSign(LPBYTE bData, DWORD dwDataSize, LPBYTE bFind, DWORD dwFindLength);
	BOOL	OnReplaceData(	LPBYTE bData, 
		DWORD dwDataSize, 
		LPBYTE bSouData,
		DWORD dwSouDataLen,
		LPBYTE bReplaceData, 
		DWORD dwReplaceLength
		);

	void	OnChangeFileTime(LPCTSTR lpszName);

	// 加载、保存配置文件
	void	LoadConfig();
	void	SaveConfig();

	CString m_strConfig;

	CString m_strType;
	CString m_strStartTime;
	CString m_strEndTime;


	afx_msg void OnBnClickedOk();
};
