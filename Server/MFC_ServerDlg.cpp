
// MFC_ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"
#include "afxdialogex.h"

#include <unordered_map>

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
	ON_MESSAGE(WM_SOCKET, SockMsg)
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
	InputDatabaseAccount();

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



void CMFCServerDlg::InputDatabaseAccount()
{
	std::ifstream fi;
	fi.open("database.txt");
	if (!fi.is_open())
	{
		MessageBox(_T("Error in opening file database.txt"), _T("Error"), MB_ICONERROR);
		return;
	}
	while (!fi.eof())
	{
		char user[100], pass[100];
		fi >> user >> pass;
		m_account.insert(std::make_pair(CString(user), CString(pass)));
	}

}

LRESULT CMFCServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
		WSACleanup();
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
		{
			
			m_client_sock.push_back(accept(wParam, NULL, NULL));
			UpdateData(FALSE);
			break;
		}
		case FD_READ:
		{
			CString src;
			std::vector<CString> res;
			if (mRecv(wParam, src) < 0)
			{
				MessageBox(_T("Error in receiving"), _T("Error"), MB_ICONERROR);
				break;
			}
			Split(src, res);
			int flag = _ttoi(res[0]);
			switch (flag)
			{
				/*Login*/
				case 1:
				{
					if (CheckLogin(res))
					{
						flag = 1;
					}
					else
					{
						flag = 0;
					}
					break;
				}

			}

			break;
		}
	
		case FD_CLOSE:
		{
			closesocket(wParam);
			CString s = _T("");
			m_list_box.AddString(_T("[+]Client disconnected"));
			UpdateData(FALSE);
			break;
		}
	}
	return 0;
}

void CMFCServerDlg::CreateSocket()
{
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

}

void CMFCServerDlg::Bind()
{
	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = htons(1234);
	m_server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int e;
	e = bind(m_server_sock, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
	if (e < 0)
	{
		MessageBox(_T("Error in binding"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}
}

void CMFCServerDlg::Listen()
{
	int e = listen(m_server_sock, SOMAXCONN);
	if (e != 0)
	{
		MessageBox(_T("Error in listening"), _T("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}

}

void CMFCServerDlg::NonBlocking()
{
	int err = WSAAsyncSelect(m_server_sock, m_hWnd, WM_SOCKET, FD_ACCEPT | FD_READ | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
}

char * CMFCServerDlg::ConvertToChar(const CString & s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CMFCServerDlg::mSend(SOCKET sk, CString Command)
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




int CMFCServerDlg::mRecv(SOCKET sk, CString &Command)
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

void CMFCServerDlg::Split(CString src, std::vector<CString> &des)
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



bool CMFCServerDlg::CheckLogin(std::vector<CString> account)
{
	bool is_login = false;

	if (m_account.find(account[1]) != m_account.end())
	{
		if (m_account[account[1]] == account[2])
		{
			is_login = true;
		}
	}
	return is_login;
}


void CMFCServerDlg::OnBnClickedBtnListen()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	GetDlgItem(IDC_BTN_LISTEN)->EnableWindow(FALSE);


	CreateSocket();
	Bind();
	Listen();
	//m_num_client = 0;

	NonBlocking();

	
	UpdateData(FALSE);
}


void CMFCServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
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
