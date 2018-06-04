// tipDlg.cpp : 实现文件
//

#include "../stdafx.h"
#include "../resource.h"
#include "tipDlg.h"


// tipDlg 对话框

IMPLEMENT_DYNAMIC(TipDlg, CDialogEx)

TipDlg::TipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

TipDlg::~TipDlg()
{
}

void TipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL TipDlg::OnInitDialog()
{
	SetWindowText(L"文件防泄漏");
	GetDlgItem(IDC_STATIC)->SetWindowText(L"加密文件不允许此操作");

	RECT rect;
	::GetWindowRect(::GetDesktopWindow(), &rect);

	SetWindowPos(nullptr, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2, 330, 170, TRUE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(TipDlg, CDialogEx)
END_MESSAGE_MAP()


// tipDlg 消息处理程序
