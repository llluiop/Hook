#include "tip.h"

#include "tipDlg.h"
#include "../resource.h"
#include <afxwin.h>

HHOOK hook = nullptr;
LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_ACTIVATE)
	{
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		auto hwnd = (HWND)wParam;

		char title[MAX_PATH] = { 0 };
		GetWindowTextA(hwnd, title, MAX_PATH);
		if (strcmp(title, "文件防泄漏") == 0)
			MoveWindow((HWND)wParam, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2, 330, 170, TRUE);
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}



void Tip::Show(const Type& type)
{
	hook = SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());

	HINSTANCE temp = AfxGetResourceHandle();
	AfxSetResourceHandle(GetModuleHandle(_T("sync_dll.dll")));


	TipDlg dlg;
	//dlg.Create(IDD_DIALOG1);

	if (type_ == UNKNOWN)
	{
		//::MessageBox(GetDesktopWindow(), L"加密文件不允许此操作                        ", L"文件防泄漏", MB_OK | MB_ICONWARNING);


		dlg.DoModal();
		type_ = type;

	}
	else
	{
		if (type_ != type)
		{
			OutputDebugStringA("type changed, not show");
		}
		else
		{
			//::MessageBox(GetDesktopWindow(), L"加密文件不允许此操作                       ", L"文件防泄漏", MB_OK | MB_ICONWARNING);
			dlg.DoModal();

			type_ = type;

		}
	}
	AfxSetResourceHandle(temp);

	UnhookWindowsHookEx(hook);

}

Tip::Tip()
{
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
}
