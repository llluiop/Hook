#include <stdafx.h>
#include "winapi.h"
#include "mhook-lib/mhook.h"

WinApiHook::CreateFileA_ WinApiHook::create_file_a_ = nullptr;
WinApiHook::CreateFileW_ WinApiHook::create_file_w_ = nullptr;

HMODULE WinApiHook::kernel32_ = nullptr;

bool WinApiHook::HookCreateFile()
{
	kernel32_ = LoadLibrary(L"Kernel32.dll");
	if (kernel32_ == nullptr)
	{
		return false;
	}

	create_file_a_ = (CreateFileA_)GetProcAddress(kernel32_, "CreateFileA");
	create_file_w_ = (CreateFileW_)GetProcAddress(kernel32_, "CreateFileW");

	if (create_file_a_ == nullptr || create_file_w_ == nullptr)
	{
		return false;
	}

	BOOL ca = Mhook_SetHook((PVOID*)&create_file_a_, _CreateFileA);
	BOOL cw = Mhook_SetHook((PVOID*)&create_file_w_, _CreateFileW);
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

HANDLE WinApiHook::_CreateFileA(
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

HANDLE WinApiHook::_CreateFileW(
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
