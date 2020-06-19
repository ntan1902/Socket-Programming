
// MFC_Server.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCServerApp:
// See MFC_Server.cpp for the implementation of this class
//

class CMFCServerApp : public CWinApp
{
public:
	CMFCServerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCServerApp theApp;
