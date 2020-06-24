
// MFC_ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_Client.h"
#include "MFC_ClientDlg.h"
#include "MFC_RegisterDlg.h"
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
	, m_file_download(_T(""))
	, m_file_upload(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_list_box_info);
	DDX_Control(pDX, IDC_PRG_CTRL, m_prg_ctrl);
	DDX_Text(pDX, IDC_EDT_USER, m_user_name);
	DDX_Text(pDX, IDC_EDT_PASS, m_pass);
	DDX_Control(pDX, IDC_LIST_FILES_SERVER, m_list_files);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CMFCClientDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDCANCEL, &CMFCClientDlg::OnBnClickedCancel)
	
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CMFCClientDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CMFCClientDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CMFCClientDlg::OnBnClickedBtnDownload)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CMFCClientDlg::OnBnClickedBtnUpload)
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
	GetDlgItem(IDC_LIST_FILES_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(FALSE);

	m_list_box_info.AddString(_T("[+]Connected to server"));

	m_list_files.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_list_files.InsertColumn(0, _T("File name"), LVCFMT_CENTER, 500);

	UpdateData(FALSE);
	InitFile();
	NonBlocking();

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

void CMFCClientDlg::SetSocket(SOCKET & sk)
{
	m_client_sock = sk;
}

void CMFCClientDlg::Login()
{
	CString account = _T("Login\r\n") + m_user_name + _T("\r\n") + m_pass + _T("\r\n");
	mSend(m_client_sock, account);
	m_prg_ctrl.ShowWindow(HIDE_WINDOW);
}

void CMFCClientDlg::NonBlocking()
{
	int err = WSAAsyncSelect(m_client_sock, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (err)
	{
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	}
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

std::string CMFCClientDlg::ConvertToString(const CString & s)
{
	CT2A pszTmp(s);
	std::string tmp(pszTmp);
	return tmp;
}

void CMFCClientDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[Len];
	memset(sendBuff, 0, Len);
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
	{
		return -1;
	}
	delete[]buffer;
	return 0;
	
}

void CMFCClientDlg::Split(CString src, std::vector<CString>& des)
{
	int p;
	int start = 0;
	while (src.Find(_T("\r\n"), start) != -1)
	{
		p = src.Find(_T("\r\n"), start);
		des.push_back(src.Mid(start, p - start));
		start = p + 2;

	}
}

void CMFCClientDlg::InitFile()
{
	m_file.push_back(_T(FILE_NAME_CLIENT1));
	m_file.push_back(_T(FILE_NAME_CLIENT2));

}

bool CMFCClientDlg::receiveFile()
{
	if (AfxSocketInit() == FALSE)
	{
		return false;
	}

	CSocket Client;
	Client.Create();

	if (Client.Connect(_T("127.0.0.1"), m_port) != 0)
	{
		CT2CA pszConvertedAnsiString(m_file_download);
		std::string file_down(pszConvertedAnsiString);
		std::ofstream fo(file_down, std::ios::binary);

		// Get size of file
		int file_size = 0;
		Client.Receive((char*)&file_size, sizeof(file_size));

		char *buffer = new char[file_size]{ 0 };
		
		int bytes_received;
		int bytes_to_receive = file_size;
		do
		{
			int buffer_size;
			buffer_size = (file_size > bytes_to_receive) ? bytes_to_receive : file_size;
			

			do
			{
				bytes_received = Client.Receive((char*)buffer, buffer_size);
			} while (bytes_received == -1);

			fo.write((char*)buffer, bytes_received);
			memset(buffer, 0, file_size);

			bytes_to_receive -= bytes_received;

		} while (bytes_to_receive > 0);

		delete[]buffer;
		fo.close();
		///-------------------------------------------------------------------------------
	}
	/*Download succeed*/
	MessageBox(m_file_download + _T(" is download successfully!"), _T("Success"), MB_OK);
	GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(TRUE);
	m_file_download = _T("");
	
	Client.Close();
	return 1;
}

bool CMFCClientDlg::sendFile()
{
	if (AfxSocketInit() == FALSE)
	{
		return FALSE;
	}

	CSocket Client;
	Client.Create();

	if (Client.Connect(_T("127.0.0.1"), m_port) != 0)
	{

		int file_size;
		std::string file_name = ConvertToString(m_file_upload);
		std::ifstream fi(file_name, std::ios::binary);
		if (!fi)
			return 0;

		// Get size of file
		fi.seekg(0, std::ios::end);
		file_size = fi.tellg();
		fi.seekg(0, std::ios::beg);

		// Send size of file
		Client.Send((char*)&file_size, sizeof(file_size));

		char *buffer = new char[file_size] { 0 };

		int bytes_sent;
		int bytes_to_send = file_size;
		do
		{
			int buffer_size;
			buffer_size = (file_size > bytes_to_send) ? bytes_to_send : file_size;
			fi.read((char*)buffer, buffer_size);

			do
			{
				bytes_sent = Client.Send((char*)buffer, buffer_size);
			} while (bytes_sent == -1);

			bytes_to_send -= bytes_sent;

		} while (bytes_to_send > 0);

		//Client.Send((char*)str.c_str(), file_size);
		delete[]buffer;
		fi.close();
		///-------------------------------------------------------------------------------
	}
	Client.Close();

	return 1;
}
void CMFCClientDlg::OnBnClickedBtnLogin()
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

	Login();

	UpdateData(FALSE);
}

void CMFCClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}
void CMFCClientDlg::OnBnClickedBtnLogout()
{
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Do you want to logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDOK)
	{

		GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_PASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_FILES_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(FALSE);

		m_user_name = _T("");
		m_pass = _T("");
		m_list_box_info.AddString(_T("Logout successfully"));
		m_list_files.DeleteAllItems();
		UpdateData(FALSE);

		mSend(m_client_sock, _T("Logout\r\n"));
	}
}

void CMFCClientDlg::OnBnClickedBtnRegister()
{
	// TODO: Add your control notification handler code here
	ShowWindow(HIDE_WINDOW);
	MFC_RegisterDlg dlg;
	dlg.SetSocket(m_client_sock);
	dlg.DoModal();
	
	ShowWindow(SW_SHOW);
	NonBlocking();
	UpdateData(FALSE);
}


void CMFCClientDlg::OnBnClickedBtnDownload()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int nItem = 0; //Represents the row number inside CListCtrl
	CString tmp;
	for (nItem = 0; nItem < m_list_files.GetItemCount(); nItem++)
	{
		BOOL bChecked = m_list_files.GetCheck(nItem);
		if (bChecked == 1)
		{
			GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(FALSE);
			m_file_download = m_list_files.GetItemText(nItem, 0);
			tmp = _T("Download\r\n") + m_file_download + _T("\r\n");
			RunProgressControl();
			mSend(m_client_sock, tmp);
			m_list_files.SetCheck(nItem, FALSE);
		}
	}
	UpdateData(FALSE);
}


void CMFCClientDlg::OnBnClickedBtnUpload()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(FALSE);
	CFileDialog t(true);
	if (t.DoModal() == IDOK)
	{
		m_file_upload = t.GetFileName();
		mSend(m_client_sock, _T("Upload\r\n") + m_file_upload + _T("\r\n"));
		RunProgressControl();
		m_prg_ctrl.ShowWindow(HIDE_WINDOW);
		MessageBox(m_file_upload + _T(" is upload successfully!"), _T("Success"), MB_OK);

		m_file_upload = _T("");

	}
	GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(TRUE);

	UpdateData(FALSE);
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
			CString src;
			std::vector<CString> res;
			if (mRecv(wParam, src) < 0)
			{
				break;
			}
			Split(src, res);

			if (res[0] == _T("Valid"))
			{
				/*Valid login*/
				m_list_box_info.AddString(_T("Login successfully!"));
				GetDlgItem(IDC_LIST_FILES_SERVER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDT_USER)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDT_PASS)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(TRUE);


			}
			else if (res[0] == _T("Invalid"))
			{
				/*Invalid login*/
				INT_PTR i = MessageBox(res[1], _T("Error"), MB_OK | MB_ICONERROR);
				if (i == IDOK)
				{
					GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_REGISTER)->EnableWindow(TRUE);

				}
			}
			else if (res[0] == _T("Login"))
			{
				m_list_box_info.AddString(res[1] + _T(" login"));

			}
			else if (res[0] == _T("OtherClientLogout"))
			{
				/*Other client logout*/
				m_list_box_info.AddString(res[1] + _T(" logout"));
			}
			
			else if (res[0] == _T("OtherClientDisConected"))
			{
				m_list_box_info.AddString(res[1] + _T(" disconected"));
			}

			else if (res[0] == _T("SendFile"))
			{
				m_list_files.InsertItem(0, res[1]);
			}
			else if (res[0] == _T("Download"))
			{
				//int iPort = _ttoi(res[1]);
				m_port = _ttoi(res[1]);
				//MessageBox(res[1]);
				if (receiveFile());
				{
					m_prg_ctrl.ShowWindow(HIDE_WINDOW);
				}
			}

			else if (res[0] == _T("Upload"))
			{
				m_port = _ttoi(res[1]);
				sendFile();
			}
			else if (res[0] == _T("DownloadError"))
			{
				MessageBox(res[1], _T("Error"), MB_OK | MB_ICONERROR);
				m_file_download = _T("");
			}

			UpdateData(FALSE);

			break;
		}
		case FD_CLOSE:
		{

			m_list_box_info.AddString(_T("[+]Server disconnected"));
			m_list_files.DeleteAllItems();
			closesocket(m_client_sock);
			GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_LOGOUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_USER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_PASS)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_UPLOAD)->EnableWindow(FALSE);
			UpdateData(FALSE);
			break;
		}

	}
	return 0;
}
