
// MFC_ClientDlg.h : header file
//

#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <string.h>

#define SIZE 1024
#define IP "192.168.1.6"
#define WM_SOCKET WM_USER+1
#pragma comment(lib, "ws2_32.lib")
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
public:
	char* ConvertToChar(const CString &s);
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	void CreateSocket();
	void Connect();
	void NonBlocking();
	void RunProgressControl();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedCancel();
protected:
	CListBox m_list_box;
	SOCKET m_client_sock;
	sockaddr_in m_server_addr;
	CProgressCtrl m_prg_ctrl;
};
