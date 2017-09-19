#include "winapi.h"
#include "mhook-lib/mhook.h"

bool WinApiHook::CreateFile()
{
	kernel32_ = LoadLibrary(L"Kernel32.dll");
	if (kernel32_ == nullptr)
	{
		return false;
	}

	create_file_a_ = (CreateFileA_)GetProcAddress(kernel32_, "CreateFileA");
	create_file_w_ = (CreateFileW_)GetProcAddress(kernel32_, "CreateFileW");

	Mhook_SetHook((PVOID*)&create_file_a_, _CreateFileA);
	Mhook_SetHook((PVOID*)&create_file_w_, _CreateFileW);

	FreeLibrary(kernel32_);
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
	return create_file_w_(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
