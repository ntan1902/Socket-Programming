// MFC_RegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "MFC_Client.h"
#include "MFC_RegisterDlg.h"
#include "MFC_ClientDlg.h"
#include "afxdialogex.h"


// MFC_RegisterDlg dialog

IMPLEMENT_DYNAMIC(MFC_RegisterDlg, CDialogEx)

MFC_RegisterDlg::MFC_RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
	, m_user_register(_T(""))
	, m_pass_register(_T(""))
{

}

MFC_RegisterDlg::~MFC_RegisterDlg()
{
}

BOOL MFC_RegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	NonBlocking();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MFC_RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_USER_REGISTER, m_user_register);
	DDX_Text(pDX, IDC_EDT_PASS_REGISTER, m_pass_register);
}

BEGIN_MESSAGE_MAP(MFC_RegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CREATE, &MFC_RegisterDlg::OnBnClickedBtnCreate)
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDCANCEL, &MFC_RegisterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// MFC_RegisterDlg message handlers
void MFC_RegisterDlg::SetSocket(SOCKET & sk)
{
	m_client_sock = sk;
}
void MFC_RegisterDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete[] sendBuff;
}

int MFC_RegisterDlg::mRecv(SOCKET sk, CString & Command)
{
	int buffLength;
	recv(sk, (char*)&buffLength, sizeof(int), 0);

	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;


	if (Command.GetLength() == 0)
		return -1;

	delete[] buffer;
	return 0;
}

void MFC_RegisterDlg::NonBlocking()
{
	int err = WSAAsyncSelect(m_client_sock, m_hWnd, WM_SOCKET, FD_READ);
	if (err)
	{
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	}
}


void MFC_RegisterDlg::OnBnClickedBtnCreate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_user_register.IsEmpty())
	{
		MessageBox(_T("You should enter the username"), _T("Error"), MB_ICONERROR);
		return;
	}
	if (m_pass_register.IsEmpty())
	{
		MessageBox(_T("You should enter the password"), _T("Error"), MB_ICONERROR);
		return;
	}
	CString r_account = _T("Register\r\n");
	r_account += m_user_register + _T("\r\n") + m_pass_register + _T("\r\n");
	mSend(m_client_sock, r_account);


	UpdateData(FALSE);
}

LRESULT MFC_RegisterDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);

	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:
		{
			CString recv_str = _T("");

			if (mRecv(m_client_sock, recv_str) < 0)
			{
				break;
			}
			if (recv_str == "used")
			{
				MessageBox(_T("This username is used. Please try another one"), _T("Error"), MB_OK);
				m_user_register = _T("");
				m_pass_register = _T("");
				UpdateData(FALSE);
			}
			else
			{
				MessageBox(_T("Account is created successfully !"), _T("Success"), MB_OK);
			}

			break;
		}
	
	}
	return 0;
}


void MFC_RegisterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	
	CDialogEx::OnCancel();
}
