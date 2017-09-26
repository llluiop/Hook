#include "winapi.h"
#include "mhook-lib/mhook.h"

WinApiHook::_CreateFileA WinApiHook::create_file_a_ = nullptr;
WinApiHook::_CreateFileW WinApiHook::create_file_w_ = nullptr;
WinApiHook::_GetSaveFileNameA WinApiHook::get_save_file_name_a_ = nullptr;
WinApiHook::_GetSaveFileNameW WinApiHook::get_save_file_name_w_ = nullptr;

HMODULE WinApiHook::kernel32_ = nullptr;
HMODULE WinApiHook::comdlg_ = nullptr;

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
