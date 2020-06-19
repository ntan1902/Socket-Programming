
// MFC_ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"
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


// CMFCServerDlg dialog



CMFCServerDlg::CMFCServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_box);
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CMFCServerDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDCANCEL, &CMFCServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMFCServerDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CMFCServerDlg message handlers

BOOL CMFCServerDlg::OnInitDialog()
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

void CMFCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCServerDlg::OnPaint()
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
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



char * CMFCServerDlg::ConvertToChar(const CString & s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

//LRESULT CMFCServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
//{
//	if (WSAGETSELECTERROR(lParam))
//	{
//		// Display the error and close the socket
//		closesocket(wParam);
//
//	}
//	switch (WSAGETSELECTEVENT(lParam))
//	{
//		case FD_ACCEPT:
//		{
//			
//			break;
//		}
//		case FD_READ:
//		{
//
//			break;
//		}
//	
//		case FD_CLOSE:
//		{
//			break;
//		}
//	}
//	return LRESULT();
//}




void CMFCServerDlg::OnBnClickedBtnListen()
{
	// TODO: Add your control notification handler code here


	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	if (WSAStartup(ver, &wsData) != 0)
	{
		MessageBox(_T("Error in starting winsock!"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}

	m_server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_server_sock == INVALID_SOCKET)
	{
		MessageBox(_T("Error in socket\n"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}

	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = htons(1234);
	m_server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int e;
	//BIND
	e = bind(m_server_sock, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
	if (e < 0)
	{
		MessageBox(_T("Error in binding"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}

	//LISTEN
	e = listen(m_server_sock, SOMAXCONN);
	if (e != 0)
	{
		MessageBox(_T( "Error in listening"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}


	//ACCEPT
	int addr_size = sizeof(m_client_addr);
	m_client_sock = accept(m_server_sock, (sockaddr*)&m_client_sock, &addr_size);



	
	m_list_box.AddString(_T("[+]Client connected"));
	
}


void CMFCServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	closesocket(m_client_sock);
	closesocket(m_server_sock);
	WSACleanup();
	CDialogEx::OnCancel();
}


void CMFCServerDlg::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
	m_list_box.ResetContent();
	UpdateData(FALSE);
}
