
// Hosts.h : Hosts Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CHostsApp:
// �йش����ʵ�֣������ Hosts.cpp
//

class CHostsApp : public CWinApp
{
public:
	CHostsApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHostsApp theApp;
