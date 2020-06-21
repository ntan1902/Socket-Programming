
// MFC_ServerDlg.h : header file
//

#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <vector>
#include <map> 
#include <time.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define WM_SOCKET WM_USER+1



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
	void InputDatabaseAccount();
	void CreateSocket();
	void Bind();
	void Listen();
	void NonBlocking();
	char* ConvertToChar(const CString &s);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	void Split(CString src, std::vector<CString> &des);
	bool CheckLogin(CString user, CString pass);
	int FindClient(SOCKET sk);
	void UpdateListClient();

	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClear();
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
protected:
	CListBox m_list_box;
	CListCtrl m_list_clients;
	SOCKET m_server_sock;

	struct SOCKET_CLIENT{
		SOCKET m_client_sock;
		CString m_user_name;
		bool m_bIsLogin = false;
	};
	std::vector<SOCKET_CLIENT> m_client;
	sockaddr_in m_server_addr;
	std::map<CString, CString> m_account;
};
