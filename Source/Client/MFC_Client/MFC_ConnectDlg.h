#pragma once
#include <iostream>
#include <stdlib.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <string>
#define IP "127.0.0.1"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

// MFC_ConnectDlg dialog

class MFC_ConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MFC_ConnectDlg)

public:
	MFC_ConnectDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MFC_ConnectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	CStatic m_title;
	CString m_ip_server;
	SOCKET m_client_sock;
	sockaddr_in m_server_addr;
	
public:
	void CreateSocket();
	void Connect();
	void RunProgressControl();
	std::string ConvertToString(const CString &s);
	void Reset();
	afx_msg void OnBnClickedBtnConnect();
protected:
	CProgressCtrl m_prg_ctrl;
public:
	afx_msg void OnBnClickedCancel();
};
