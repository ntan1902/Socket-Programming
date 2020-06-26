#pragma once

#include <iostream>
#include <stdlib.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <vector>
// MFC_RegisterDlg dialog
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

class MFC_RegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MFC_RegisterDlg)

public:
	MFC_RegisterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MFC_RegisterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void SetSocket(SOCKET &sk);
protected:
	SOCKET m_client_sock;
	CString m_user_register;
	CString m_pass_register;
public:
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);
	void NonBlocking();
	afx_msg void OnBnClickedBtnCreate();
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedCancel();
};
