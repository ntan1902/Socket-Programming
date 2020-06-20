
// MFC_ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Client.h"
#include "MFC_ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCClientDlg dialog



CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_CLIENT_DIALOG, pParent)
	, m_user_name(_T(""))
	, m_pass(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_list_box);
	DDX_Control(pDX, IDC_PRG_CTRL, m_prg_ctrl);
	DDX_Text(pDX, IDC_EDT_USER, m_user_name);
	DDX_Text(pDX, IDC_EDT_PASS, m_pass);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMFCClientDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDCANCEL, &CMFCClientDlg::OnBnClickedCancel)
	
END_MESSAGE_MAP()


// CMFCClientDlg message handlers

BOOL CMFCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CMFCClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
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
		

		
		break;
	}
	case FD_CLOSE:
	{
		closesocket(m_client_sock);
		m_list_box.AddString(_T("[+]Server disconnected"));
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}

void CMFCClientDlg::CreateSocket()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	if (WSAStartup(ver, &wsData) != 0) {
		MessageBox(_T("Error starting winsock!"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);

		return;
	}

	m_client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_client_sock == INVALID_SOCKET)
	{
		MessageBox(_T("Error in socket"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);

		return;
	}
}

void CMFCClientDlg::Connect()
{
	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = htons(1234);

	inet_pton(AF_INET, IP, &m_server_addr.sin_addr);
	int e = connect(m_client_sock, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
	
	
	m_prg_ctrl.ShowWindow(HIDE_WINDOW);
	if (e == -1)
	{
		MessageBox(_T("Error in connecting"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);

		return;
	}
	m_list_box.AddString(_T("[+]Connected to server"));

	UpdateData(FALSE);
}

void CMFCClientDlg::NonBlocking()
{
	int err = WSAAsyncSelect(m_client_sock, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
}

void CMFCClientDlg::RunProgressControl()
{
	m_prg_ctrl.ShowWindow(SW_SHOW);
	m_prg_ctrl.SetRange(0, 1000);

	for (int i = 0; i < 10000 / 2; i++)
	{
		m_prg_ctrl.SetPos(i);
	}
}

char * CMFCClientDlg::ConvertToChar(const CString & s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CMFCClientDlg::mSend(SOCKET sk, CString Command)
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

int CMFCClientDlg::mRecv(SOCKET sk, CString & Command)
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

	delete[]buffer;
	return 0;
}

void CMFCClientDlg::Split(CString src, std::vector<CString>& des)
{
	int p;
	int start = 0;
	// 1\r\nNtan\r\n1902\r\n
	while (src.Find(_T("\r\n"), start) != -1)
	{
		p = src.Find(_T("\r\n"), start);
		des.push_back(src.Mid(start, p - start));
		start = p + 2;

	}
}



void CMFCClientDlg::OnBnClickedBtnConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_user_name.IsEmpty())
	{
		MessageBox(_T("You should enter the username"), _T("Error"), MB_ICONERROR);
		return;
	}
	if (m_pass.IsEmpty())
	{
		MessageBox(_T("You should enter the password"), _T("Error"), MB_ICONERROR);
		return;
	}
	

	RunProgressControl();

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);

	CreateSocket();
	
	Connect();

	CString account = _T("1\r\n") + m_user_name + _T("\r\n") + m_pass + _T("\r\n");
	mSend(m_client_sock, account);

	NonBlocking();

	UpdateData(FALSE);
}



void CMFCClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	closesocket(m_client_sock);
	CDialogEx::OnCancel();
}
