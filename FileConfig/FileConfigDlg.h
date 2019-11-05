// FileConfigDlg.h : ͷ�ļ�
//

#pragma once


// CFileConfigDlg �Ի���
class CFileConfigDlg : public CDialog
{
// ����
public:
	CFileConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILECONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    

	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	// ���ء����������ļ�
	void	LoadConfig();
	void	SaveConfig();

	CString m_strConfig;

	CString m_strType;
	CString m_strStartTime;
	CString m_strEndTime;


	afx_msg void OnBnClickedOk();
};
