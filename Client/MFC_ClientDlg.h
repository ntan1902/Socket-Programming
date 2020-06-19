
// MFC_ClientDlg.h : header file
//

#pragma once
#include <iostream>
#include <stdlib.h>
#include <afxsock.h>
#include <string.h>
#include <time.h>

#pragma warning(disable : 4996)

// CMFCClientDlg dialog
class CMFCClientDlg : public CDialogEx
{
// Construction
public:
	CMFCClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CLIENT_DIALOG };
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
	CSocket m_client;
public:
	char* ConvertToChar(const CString &s);
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedCancel();
protected:
	CListBox m_list_box;

};
