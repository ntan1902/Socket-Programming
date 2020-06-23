
// MFC_ServerDlg.h : header file
//

#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
//#include <string.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <fstream>
#include <vector>
#include <map> 
#include <time.h>
#include <sstream>
#include <afxcmn.h>
#include <afxwin.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define WM_SOCKET WM_USER+1

#define FILE_NAME_SERVER1 "Data_Server1.txt"
#define FILE_NAME_SERVER2 "Data_Server2.txt"
#define FILE_NAME_SERVER3 "Data_Server3.txt"
#define FILE_NAME_SERVER4 "Lab.docx"

#define FILE_NAME_DATABASE "database.txt"

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
	void OutputDatabaseAccount(CString user, CString pass);
	void CreateSocket();
	void Bind();
	void Listen();
	void NonBlocking();
	char* ConvertToChar(const CString &s);
	std::string ConvertToString(const CString &s);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	void Split(CString src, std::vector<CString> &des);
	bool CheckLogin(CString user, CString pass);
	bool CheckRegister(CString user);
	int FindClient(SOCKET sk);
	void UpdateListClient();
	void InitFile();
	void UpdateListFile();
	void UploadPathFile(CString path);
	void SendFileNameToClient(SOCKET sk);
	std::string getNameOfFile(std::string s);
	afx_msg void OnBnClickedBtnListen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClear();
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	static UINT sendFile(LPVOID pParam);
	static UINT receiveFile(LPVOID pParam);

	static std::string file_name;
protected:
	int buffLength;
	CListBox m_list_box_info;
	CListCtrl m_list_clients;
	CListCtrl m_list_files;
	SOCKET m_server_sock;

	struct SOCKET_CLIENT{
		SOCKET m_client_sock;
		CString m_user_name;
		bool m_bIsLogin = false;
	};
	std::vector<SOCKET_CLIENT> m_client;
	sockaddr_in m_server_addr;
	std::map<CString, CString> m_account;
	std::vector<CString> m_file;
	std::vector<CString> res;
	
public:
	afx_msg void OnBnClickedBtnAddFiles();
};