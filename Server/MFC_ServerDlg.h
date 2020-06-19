
// MFC_ServerDlg.h : header file
//

#pragma once
#include <iostream>
#include <stdlib.h>
#include <afxsock.h>
#include <string.h>
#include <time.h>

#pragma warning(disable : 4996)
// CMFCServerDlg dialog
class CMFCServerDlg : public CDialogEx
{
// Construction
public:
	CMFCServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	char* ConvertToChar(const CString &s);
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClear();
protected:
	CListBox m_list_box;
	CSocket m_server, m_client;

};
