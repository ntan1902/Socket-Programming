
// MFC_ClientDlg.h : header file
//

#pragma once
#include <iostream>
#include <fstream>
//#include <stdlib.h>
//#include <string.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define WM_SOCKET WM_USER+1


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
	void SetSocket(SOCKET &sk);
	void Login();
	void NonBlocking();
	void RunProgressControl();
	char* ConvertToChar(const CString &s);
	std::string ConvertToString(const CString &s);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	void Split(CString src, std::vector<CString> &des);

	bool receiveFile();
	bool sendFile();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnRegister();

	afx_msg void OnBnClickedBtnDownload();
	afx_msg void OnBnClickedBtnUpload();
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
protected:
	int buffLength;
	CListBox m_list_box_info;
	SOCKET m_client_sock;
	CProgressCtrl m_prg_ctrl;
	CString m_user_name;
	CString m_pass;
	CString m_file_download;
	CString m_file_upload;
	CListCtrl m_list_files;
	
	/*port to exchange data with server*/
	int m_port;

};
