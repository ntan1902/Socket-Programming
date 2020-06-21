// MFC_RegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "MFC_Client.h"
#include "MFC_RegisterDlg.h"
#include "afxdialogex.h"


// MFC_RegisterDlg dialog

IMPLEMENT_DYNAMIC(MFC_RegisterDlg, CDialogEx)

MFC_RegisterDlg::MFC_RegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
{

}

MFC_RegisterDlg::~MFC_RegisterDlg()
{
}

BOOL MFC_RegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MFC_RegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MFC_RegisterDlg, CDialogEx)
END_MESSAGE_MAP()


// MFC_RegisterDlg message handlers
