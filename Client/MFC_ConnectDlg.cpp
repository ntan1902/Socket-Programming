// MFC_ConnectDlg.cpp : implementation file
//

#include "pch.h"
#include "MFC_Client.h"
#include "MFC_ConnectDlg.h"
#include "MFC_ClientDlg.h"
#include "afxdialogex.h"


// MFC_ConnectDlg dialog

IMPLEMENT_DYNAMIC(MFC_ConnectDlg, CDialogEx)

MFC_ConnectDlg::MFC_ConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONNECT, pParent)
	, m_ip_server(_T(""))
{

}

MFC_ConnectDlg::~MFC_ConnectDlg()
{
}

void MFC_ConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_title);
	DDX_Text(pDX, IDC_EDT_IP, m_ip_server);
	DDX_Control(pDX, IDC_PRG_CONTROL_CONNECT, m_prg_ctrl);
}

BOOL MFC_ConnectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CFont font;
	font.CreatePointFont(10, _T("Arial"));

	GetDlgItem(IDC_STATIC_TITLE)->SetFont(&font);

	m_ip_server = IP;
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(MFC_ConnectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &MFC_ConnectDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()


// MFC_ConnectDlg message handlers
void MFC_ConnectDlg::CreateSocket()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	if (WSAStartup(ver, &wsData) != 0) {
		INT_PTR i = MessageBox(_T("Error starting winsock!"), _T("Error"), MB_OK | MB_ICONERROR);
		if (i == IDOK)
		{
			WSACleanup();
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
			return;
		}
	}

	m_client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_client_sock == INVALID_SOCKET)
	{
		INT_PTR i = MessageBox(_T("Error in socket"), _T("Error"), MB_OK | MB_ICONERROR);
		if (i == IDOK)
		{
			WSACleanup();
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
			return;
		}
	}
}

void MFC_ConnectDlg::Connect()
{
	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = htons(1234);

	char *ip = ConvertToChar(m_ip_server);
	inet_pton(AF_INET, ip, &m_server_addr.sin_addr);
	delete[]ip;

	int e = connect(m_client_sock, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
	
	m_prg_ctrl.ShowWindow(HIDE_WINDOW);

	if (e == -1)
	{
		INT_PTR i = MessageBox(_T("Error in connecting"), _T("Error"), MB_OK | MB_ICONERROR);
		if (i == IDOK)
		{
			WSACleanup();
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
			return;
		}
	}

	/*Change Dialog*/
	CMFCClientDlg dlg;
	dlg.GetSocket(m_client_sock);
	ShowWindow(HIDE_WINDOW);
	dlg.DoModal();
	ShowWindow(SW_SHOW);
	Reset();

	UpdateData(FALSE);
}

void MFC_ConnectDlg::RunProgressControl()
{
	m_prg_ctrl.ShowWindow(SW_SHOW);
	m_prg_ctrl.SetRange(0, 1000);

	for (int i = 0; i < 10000 / 2; i++)
	{
		m_prg_ctrl.SetPos(i);
	}
}

char * MFC_ConnectDlg::ConvertToChar(const CString & s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void MFC_ConnectDlg::Reset()
{
	closesocket(m_client_sock);
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);

}


void MFC_ConnectDlg::OnBnClickedBtnConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	RunProgressControl();
	CreateSocket();
	Connect();

}
