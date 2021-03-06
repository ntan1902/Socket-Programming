
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

std::string file_name;
int iPort = 10000;
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
	DDX_Control(pDX, IDC_LIST_INFO, m_list_box_info);
	DDX_Control(pDX, IDC_LIST_CLIENTS, m_list_clients);
	DDX_Control(pDX, IDC_LIST_FILES, m_list_files);
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CMFCServerDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDCANCEL, &CMFCServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMFCServerDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_ADD_FILES, &CMFCServerDlg::OnBnClickedBtnAddFiles)
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
	CreateSocket();
	Bind();


	m_list_clients.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list_clients.InsertColumn(0, _T("Client Name"),LVCFMT_LEFT, 110);
	m_list_clients.InsertColumn(1, _T("Connection"), LVCFMT_LEFT, 110);
	m_list_clients.InsertColumn(2, _T("Status"), LVCFMT_LEFT, 110);


	m_list_files.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_list_files.InsertColumn(0, _T("File name"), LVCFMT_CENTER, 500);

	//InitFile();
	UpdateListFile();

	NonBlocking();

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
	m_account.clear();
	std::ifstream fi;
	fi.open("database.txt");
	if (!fi.is_open())
	{
		MessageBox(_T("Error in opening file database.txt"), _T("Error"), MB_ICONERROR);
		return;
	}
	while (!fi.eof())
	{
		std::string user, pass;
		fi >> user >> pass;
		m_account.insert(std::make_pair(CString(user.c_str()), CString(pass.c_str())));
	}

}

void CMFCServerDlg::OutputDatabaseAccount(CString user, CString pass)
{
	m_account.insert(std::make_pair(user, pass)); // 1: username, 2: pass

	std::ofstream fo;
	fo.open("database.txt", std::ios::app);
	if (!fo.is_open())
	{
		MessageBox(_T("Error in opening file database.txt"), _T("Error"), MB_ICONERROR);
		return;
	}
	else
	{
		/*std::map<CString, CString>::iterator itr;
		for (itr = m_account.begin(); itr != m_account.end(); ++itr)
		{
			fo << ConvertToChar(itr->first) << " " << ConvertToChar(itr->second) << '\n';
		}*/

		std::string u = ConvertToString(user);
		std::string p = ConvertToString(pass);
		fo << "\n" << u << " " << p;
		
	}
	fo.close();
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
	m_server_addr.sin_addr.S_un.S_addr = htons(INADDR_ANY);

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

std::string CMFCServerDlg::ConvertToString(const CString & s)
{
	CT2CA pszTmp(s);
	std::string tmp(pszTmp);
	return tmp;
}

void CMFCServerDlg::mSend(SOCKET sk, CString Command)
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




int CMFCServerDlg::mRecv(SOCKET sk, CString &Command)
{
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

bool CMFCServerDlg::CheckLogin(CString user, CString pass)
{
	bool is_login = false;

	if (m_account[user] == pass)
	{
		is_login = true;
	}

	return is_login;
}

bool CMFCServerDlg::CheckRegister(CString user)
{
	bool is_same = false;
	std::map<CString, CString>::iterator it;
	for (it = m_account.begin(); it != m_account.end(); it++)
	{
		if (it->first == user)
		{
			is_same = true;
		}
	}
	return is_same;
}

int CMFCServerDlg::FindClient(SOCKET sk)
{
	for (int i = 0; i < m_client.size(); i++)
	{
		if (m_client[i].m_client_sock == sk)
		{
			return i;
		}
	}
	return -1;
}

void CMFCServerDlg::UpdateListClient()
{
	m_list_clients.DeleteAllItems();
	int row;
	for (int i = 0; i < m_client.size(); i++)
	{
		row = m_list_clients.InsertItem(0, m_client[i].m_user_name);
		m_list_clients.SetItemText(row, 1, _T("Connected"));
		if (!m_client[i].m_bIsLogin)
		{
			m_list_clients.SetItemText(row, 2, _T("Logout"));
		}
		else
		{
			m_list_clients.SetItemText(row, 2, _T("Login"));
		}
	}
}


void CMFCServerDlg::UpdateListFile()
{
	m_list_files.DeleteAllItems();
	for (int i = 0; i < m_file.size(); i++)
	{
		m_list_files.InsertItem(0, m_file[i]);
	}
	UpdateData(FALSE);
}

void CMFCServerDlg::SendFileNameToClient(SOCKET sk)
{
	
	for (int i = 0; i < m_file.size(); i++)
	{
		CString tmp = _T("SendFile\r\n") + m_file[i] + _T("\r\n");
		mSend(sk, tmp);
	}
	
}

void CMFCServerDlg::SendFileNameToAllClient(CString file)
{
	for (int i = 0; i < m_client.size(); i++)
	{
		if (m_client[i].m_bIsLogin)
			mSend(m_client[i].m_client_sock, _T("SendFile\r\n") + file + _T("\r\n"));
	}
}

std::string CMFCServerDlg::GetFilePath(std::string s)
{
	for(int i = 0; i < m_file_path.size(); i++)	
	{ 
		std::string path = ConvertToString(m_file_path[i]);
		if (path.find(s) != std::string::npos)
			return path;
	}
	return "";
}


void CMFCServerDlg::OnBnClickedBtnListen()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	GetDlgItem(IDC_BTN_LISTEN)->EnableWindow(FALSE);

	Listen();


	
	UpdateData(FALSE);
}


void CMFCServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CMFCServerDlg::OnBnClickedBtnClear()
{
	// TODO: Add your control notification handler code here
	m_list_box_info.ResetContent();
	UpdateData(FALSE);
}

void CMFCServerDlg::OnBnClickedBtnAddFiles()
{
	// TODO: Add your control notification handler code here
	CFileDialog t(true);
	if (t.DoModal() == IDOK)
	{
		CString tmp = t.GetFileName();
		m_file_path.push_back(t.GetPathName());
		m_list_files.InsertItem(0, tmp);
		m_file.push_back(tmp);

		/*Send file to all clients*/
		SendFileNameToAllClient(tmp);
	}
}

UINT CMFCServerDlg::sendFile(LPVOID pParam)
{
	if (AfxSocketInit() == FALSE)
	{

		return FALSE;
	}

	CSocket Server; //cha


	
	if (Server.Create(iPort, SOCK_STREAM, NULL) == 0)
	{
		return FALSE;
	}
	else
	{

		if (Server.Listen(32) == FALSE)
		{

			Server.Close();
			return FALSE;
		}

	}


	CSocket Client;
	if (Server.Accept(Client))
	{
		int file_size;

		std::ifstream fi(file_name, std::ios::binary);
		if (!fi)
			return 0;

		// Get size of file
		fi.seekg(0, std::ios::end);
		file_size = fi.tellg();
		fi.seekg(0, std::ios::beg);

		// Send size of file
		Client.Send((char*)&file_size, sizeof(file_size));

		// Send data
		/*std::stringstream strStream;
		strStream << fi.rdbuf();
		std::string str = strStream.str();*/
		char *buffer = new char[file_size];

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
		///------------------------------------------------------------------------------------
	}
	Client.Close();
	Server.Close();
}

UINT CMFCServerDlg::receiveFile(LPVOID pParam)
{
	if (AfxSocketInit() == FALSE)
	{

		return FALSE;
	}

	CSocket Server;
	
	if (Server.Create(iPort, SOCK_STREAM, NULL) == 0)
	{

		return FALSE;
	}
	else
	{

		if (Server.Listen(32) == FALSE)
		{
			Server.Close();
			return FALSE;
		}

	}


	CSocket Client;
	if (Server.Accept(Client))
	{

		std::ofstream fo(file_name, std::ios::binary);

		// Get size of file
		int file_size = 0;
		Client.Receive((char*)&file_size, sizeof(file_size));

		char *buffer = new char[file_size];

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
		///------------------------------------------------------------------------------------
	}
	Client.Close();
	Server.Close();
}




LRESULT CMFCServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
		{
			SOCKET_CLIENT s;
			s.m_client_sock = accept(wParam, NULL, NULL);

			srand((unsigned)time(NULL));
			int R = rand();
			CString guest_R;
			guest_R.Format(_T("%d"), R);
			s.m_user_name = _T("Guest ") + guest_R;

			m_client.push_back(s);

			/*Update list clients*/
			UpdateListClient();


			UpdateData(FALSE);
			break;
		}
		case FD_READ:
		{
			int i = FindClient(wParam);
			CString src;
			if (mRecv(wParam, src) < 0)
			{
				break;
			}
			Split(src, res);


			if (res[0] == _T("Login"))
			{
				/*Login*/
				if (CheckLogin(res[1], res[2]))
				{
					mSend(wParam, _T("Valid\r\n"));
					m_client[i].m_user_name = res[1];
					m_client[i].m_bIsLogin = true;
					m_list_box_info.AddString(res[1] + _T(" login"));

					for (int j = 0; j < m_client.size(); j++)
					{
						if (j != i && m_client[j].m_bIsLogin)
						{
							mSend(m_client[j].m_client_sock, _T("Login\r\n") + m_client[i].m_user_name + _T("\r\n"));
						}
					}
					UpdateListClient();
					
					/*Send all file of server to client*/
					SendFileNameToClient(wParam);

				}
				else
				{
					mSend(wParam, _T("Invalid\r\nInvalid login, please try again\r\n"));
					if (i >= 0)
					{
						m_client[i].m_bIsLogin = false;
					}
				}
				UpdateData(FALSE);
			}

			else if (res[0] == _T("Logout"))
			{
				/*Logout*/
				if (m_client[i].m_bIsLogin)
				{
					m_client[i].m_bIsLogin = false;

					m_list_box_info.AddString(m_client[i].m_user_name + _T(" logout"));

					for (int j = 0; j < m_client.size(); j++)
					{
						if (j != i && m_client[j].m_bIsLogin)
						{
							mSend(m_client[j].m_client_sock, _T("OtherClientLogout\r\n") + m_client[i].m_user_name + _T("\r\n"));
						}
					}

					srand((unsigned)time(NULL));
					int R = rand();
					CString guest_R;
					guest_R.Format(_T("%d"), R);
					m_client[i].m_user_name = _T("Guest ") + guest_R;
					UpdateListClient();


				}
				UpdateData(FALSE);
			}

			else if (res[0] == _T("Register"))
			{
				if (!CheckRegister(res[1]))
				{
					mSend(wParam, _T("unused"));
					OutputDatabaseAccount(res[1], res[2]);
					InputDatabaseAccount();
				}
				else
				{
					mSend(wParam, _T("used"));
				}
				
			}

			else if (res[0] == _T("Download"))
			{
				bool file_exist = false;
				for (int k = 0; k < m_file.size(); k++)
				{
					
					if (m_file[k].Compare(res[1]) == 0)
					{
						file_exist = true;
					}
				}

				if (file_exist)
				{
					iPort++;
					std::string s_port = std::to_string(iPort);
					CString cs_port(s_port.c_str());

					mSend(wParam, _T("Download\r\n") + cs_port + _T("\r\n"));
					
					std::string path = ConvertToString(res[1]);
					file_name = GetFilePath(path);
					AfxBeginThread(sendFile, 0);
					
				}
				else
				{
					mSend(wParam, _T("DownloadError\r\nThe file name doesn't exist\r\n"));
				
					break;
				}

				m_list_box_info.AddString(m_client[i].m_user_name + _T(" download file ") + res[1]);
			}

			else if (res[0] == _T("Upload"))
			{
				/*Check if there is any client is uploading*/
				bool bCanUpload = true;
				for(int j = 0 ; j < m_client.size(); j++)
				{
					if (m_client[j].m_bIsUpload)
						bCanUpload = false;
				}
				if (bCanUpload)
				{
					m_client[i].m_bIsUpload = true;

					bool add_file = true;
					for (int k = 0; k < m_file.size(); k++)
					{
						if (m_file[k] == res[1])
						{
							add_file = false;
						}
					}
					if (add_file)
					{
						m_file.push_back(res[1]);
					}

					iPort++;
					std::string s_port = std::to_string(iPort);
					CString cs_port(s_port.c_str());

					mSend(wParam, _T("Upload\r\n") + cs_port + _T("\r\n"));

					file_name = ConvertToString(res[1]);
					AfxBeginThread(receiveFile, 0);

					m_list_box_info.AddString(m_client[i].m_user_name + _T(" upload file ") + res[1]);
					UpdateListFile();

					/*Send upload file to all clients*/
					SendFileNameToAllClient(res[1]);
				}
				else
				{
					mSend(wParam, _T("UploadError\r\nThere is other client uploading\r\n"));

				}
			}

			else if (res[0] == "UploadSuccess")
			{
				m_client[i].m_bIsUpload = false;
			}
			res.clear();
			break;
		}

		case FD_CLOSE:
		{
			UpdateData();
			int i = FindClient(wParam);
			if (i >= 0)
			{
				m_list_box_info.AddString(m_client[i].m_user_name + _T(" disconnected"));
				
				for (int j = 0; j < m_client.size(); j++)
				{
					if (j != i && m_client[j].m_bIsLogin)
					{
						mSend(m_client[j].m_client_sock, _T("OtherClientDisConected\r\n") + m_client[i].m_user_name + _T("\r\n"));
					}
				}

				m_client.erase(m_client.begin() + i);
				UpdateListClient();
				closesocket(wParam);
			}
			UpdateData(FALSE);
			break;
		}
	}
	return 0;
}
