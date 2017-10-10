#include "winapi.h"
#include "mhook-lib/mhook.h"
#include <wchar.h>

WinApiHook::_CreateFileA WinApiHook::create_file_a_ = nullptr;
WinApiHook::_CreateFileW WinApiHook::create_file_w_ = nullptr;
WinApiHook::_GetSaveFileNameA WinApiHook::get_save_file_name_a_ = nullptr;
WinApiHook::_GetSaveFileNameW WinApiHook::get_save_file_name_w_ = nullptr;
WinApiHook::_SetClipboardData WinApiHook::set_clipboard_data_ = nullptr;

HMODULE WinApiHook::kernel32_ = nullptr;
HMODULE WinApiHook::comdlg_ = nullptr;
HMODULE WinApiHook::user32_ = nullptr;

const int len_limit_copy = 2;

bool WinApiHook::HookCreateFile()
{
	kernel32_ = LoadLibrary(L"Kernel32.dll");
	if (kernel32_ == nullptr)
	{
		return false;
	}

	create_file_a_ = (_CreateFileA)GetProcAddress(kernel32_, "CreateFileA");
	create_file_w_ = (_CreateFileW)GetProcAddress(kernel32_, "CreateFileW");

	if (create_file_a_ == nullptr || create_file_w_ == nullptr)
	{
		return false;
	}

	BOOL ca = Mhook_SetHook((PVOID*)&create_file_a_, MyCreateFileA);
	BOOL cw = Mhook_SetHook((PVOID*)&create_file_w_, MyCreateFileW);
	if (ca == FALSE || cw == FALSE)
	{
		return false;
	}

	return true;
}

bool WinApiHook::UnHookCreateFile()
{
	if (create_file_a_)
		Mhook_Unhook((PVOID*)&create_file_a_);
	
	if (create_file_w_)
		Mhook_Unhook((PVOID*)&create_file_w_);

	return !!FreeLibrary(kernel32_);
}

bool WinApiHook::HookSaveFileAs()
{
	comdlg_ = LoadLibrary(L"comdlg32.dll");
	if (comdlg_ == nullptr)
	{
		return false;
	}

	get_save_file_name_a_ = (_GetSaveFileNameA)GetProcAddress(comdlg_, "GetSaveFileNameA");
	get_save_file_name_w_ = (_GetSaveFileNameW)GetProcAddress(comdlg_, "GetSaveFileNameW");

	if (get_save_file_name_a_ == nullptr || get_save_file_name_w_ == nullptr)
	{
		return false;
	}

	BOOL ga = Mhook_SetHook((PVOID*)&get_save_file_name_a_, MyGetSaveFileNameA);
	BOOL gw = Mhook_SetHook((PVOID*)&get_save_file_name_w_, MyGetSaveFileNameW);
	if (ga == FALSE || gw == FALSE)
	{
		return false;
	}

	return true;
}

bool WinApiHook::UnHookSaveFileAs()
{
	if (get_save_file_name_a_)
		Mhook_Unhook((PVOID*)&get_save_file_name_a_);

	if (get_save_file_name_w_)
		Mhook_Unhook((PVOID*)&get_save_file_name_w_);

	return !!FreeLibrary(comdlg_);
}

bool WinApiHook::HookSetClipboardData()
{
	user32_ = LoadLibrary(L"user32.dll");
	if (user32_ == nullptr)
	{
		return false;
	}

	set_clipboard_data_ = (_SetClipboardData)GetProcAddress(user32_, "SetClipboardData");
	if (set_clipboard_data_ == nullptr)
	{
		return false;
	}

	return !!Mhook_SetHook((PVOID*)&set_clipboard_data_, MySetClipboardData);
}

bool WinApiHook::UnHookSetClipboardData()
{
	if (set_clipboard_data_)
	{
		Mhook_Unhook((PVOID*)&set_clipboard_data_);
	}
	return !!FreeLibrary(user32_);
}

HANDLE WinApiHook::MyCreateFileA(
	_In_ LPCSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile)
{
	if (lpFileName != "")
	{
		OutputDebugStringA(lpFileName);
	}
	return create_file_a_(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE WinApiHook::MyCreateFileW(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile)
{
	if (lpFileName != L"")
	{
		OutputDebugStringW(lpFileName);
	}
	return create_file_w_(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL WinApiHook::MyGetSaveFileNameA(LPOPENFILENAME lpofn)
{
	return FALSE;
}

BOOL WinApiHook::MyGetSaveFileNameW(LPOPENFILENAME lpofn)
{
	return FALSE;
}

HANDLE WinApiHook::MySetClipboardData(UINT uFormat, HANDLE hMem)
{
	if (hMem == nullptr)
	{
		return set_clipboard_data_(uFormat, hMem);
	}
	//office word format
	if (uFormat == 49161)
	{
		IDataObject* src = (IDataObject*)GlobalLock(hMem);
		return nullptr;
	}
	else
	{
		return set_clipboard_data_(uFormat, hMem);
	}
	if (uFormat == 49171)
	{
	}
	size_t size = GlobalSize(hMem);
	if (size > len_limit_copy)
	{
		if (uFormat == CF_TEXT)
		{
			char limit[len_limit_copy] = { 0 };
			char* src = (char*)GlobalLock(hMem);
			if (src != nullptr)
			{
				memcpy(limit, src, len_limit_copy);
				limit[len_limit_copy - 1] = 0;
				memset(src, 0, size);
				memcpy(src, limit, len_limit_copy);
				GlobalUnlock(hMem);
			}

		}
		else if (uFormat == CF_UNICODETEXT)
		{
			wchar_t limit[len_limit_copy] = { 0 };
			wchar_t* src = (wchar_t*)GlobalLock(hMem);
			if (src != nullptr)
			{
				wmemcpy(limit, src, len_limit_copy);
				limit[len_limit_copy - 1] = 0;
				wmemset(src, 0, size);
				wmemcpy(src, limit, len_limit_copy);
				GlobalUnlock(hMem);
			}
		}
		else if (uFormat == CF_OEMTEXT)
		{

		}
	}

	return set_clipboard_data_(uFormat, hMem);;
}
