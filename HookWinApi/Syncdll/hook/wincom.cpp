#include "wincom.h"
#include "mhook-lib/mhook.h"


IFileDialog* WinComHook::iFileDialog_ = nullptr;
WinComHook::_Show WinComHook::show_;

HRESULT WinComHook::MyShow(IFileDialog * pIFileDialog, HWND hwndOwner)
{
	return S_FALSE;
}

bool WinComHook::HookSaveFileAs()
{
	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_HANDLER | CLSCTX_INPROC_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
		IID_PPV_ARGS(&iFileDialog_));

	if (!SUCCEEDED(hr))
	{
		return false;
	}

	show_ = (_Show)*((int*)*(int*)iFileDialog_ + 3);
	if (show_)
	{
		return !!Mhook_SetHook((PVOID*)&show_, MyShow);
	}
	else
	{
		return false;
	}
}

bool WinComHook::UnHookSaveFileAs()
{
	if (show_)
	{
		Mhook_Unhook((PVOID*)&show_);
	}

	return true;
}
