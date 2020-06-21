#pragma once


// MFC_RegisterDlg dialog

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
};
