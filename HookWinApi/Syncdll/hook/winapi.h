#pragma once

#include <Windows.h>
#include <commdlg.h>

class WinApiHook
{
public:
	typedef HANDLE (WINAPI* _CreateFileA)(
			_In_ LPCSTR lpFileName,
			_In_ DWORD dwDesiredAccess,
			_In_ DWORD dwShareMode,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_ DWORD dwCreationDisposition,
			_In_ DWORD dwFlagsAndAttributes,
			_In_opt_ HANDLE hTemplateFile
		);

	typedef	HANDLE (WINAPI* _CreateFileW)(
			_In_ LPCWSTR lpFileName,
			_In_ DWORD dwDesiredAccess,
			_In_ DWORD dwShareMode,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_ DWORD dwCreationDisposition,
			_In_ DWORD dwFlagsAndAttributes,
			_In_opt_ HANDLE hTemplateFile
		);

	typedef BOOL(WINAPI* _GetSaveFileNameA)(_Inout_ LPOPENFILENAME lpofn);
	typedef BOOL(WINAPI* _GetSaveFileNameW)(_Inout_ LPOPENFILENAME lpofn);

	typedef HANDLE(WINAPI* _SetClipboardData)(	_In_ UINT uFormat, _In_opt_ HANDLE hMem);


	static bool HookCreateFile();
	static bool UnHookCreateFile();

	static bool HookSaveFileAs();
	static bool UnHookSaveFileAs();

	static bool HookSetClipboardData();
	static bool UnHookSetClipboardData();

private:
	static HANDLE WINAPI MyCreateFileA(
		_In_ LPCSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
		);

	static HANDLE WINAPI MyCreateFileW(
		_In_ LPCWSTR lpFileName,
		_In_ DWORD dwDesiredAccess,
		_In_ DWORD dwShareMode,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		_In_ DWORD dwCreationDisposition,
		_In_ DWORD dwFlagsAndAttributes,
		_In_opt_ HANDLE hTemplateFile
		);

	static BOOL WINAPI MyGetSaveFileNameA(_Inout_ LPOPENFILENAME lpofn);
	static BOOL WINAPI MyGetSaveFileNameW(_Inout_ LPOPENFILENAME lpofn);

	static HANDLE WINAPI MySetClipboardData(_In_ UINT uFormat, _In_opt_ HANDLE hMem);


	static _CreateFileA create_file_a_;
	static _CreateFileW create_file_w_;

	static _GetSaveFileNameA get_save_file_name_a_;
	static _GetSaveFileNameW get_save_file_name_w_;

	static _SetClipboardData set_clipboard_data_;

	static HMODULE kernel32_;
	static HMODULE comdlg_;
	static HMODULE user32_;
};