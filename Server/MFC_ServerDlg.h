
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
#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable : 4996)

#define WM_SOCKET WM_USER+1
//#define SIZE_CLIENT 100
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
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	void CreateSocket();
	void Bind();
	void Listen();
	void NonBlocking();
	char* ConvertToChar(const CString &s);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	void Split(CString src, std::vector<CString> &des);
	bool CheckLogin(std::vector<CString> account);

	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClear();
protected:
	CListBox m_list_box;
	SOCKET m_server_sock;

	//int m_num_client;
	std::vector<SOCKET> m_client_sock;
	sockaddr_in m_server_addr;
	std::map<CString, CString> m_account;
};
