#include "winapi.h"
#include "mhook-lib/mhook.h"
#include "../tip/tip.h"

#include <wchar.h>
#include <string>

WinApiHook::_CreateFileA WinApiHook::create_file_a_ = nullptr;
WinApiHook::_CreateFileW WinApiHook::create_file_w_ = nullptr;

WinApiHook::_GetSaveFileNameA WinApiHook::get_save_file_name_a_ = nullptr;
WinApiHook::_GetSaveFileNameW WinApiHook::get_save_file_name_w_ = nullptr;

WinApiHook::_SetClipboardData WinApiHook::set_clipboard_data_ = nullptr;
WinApiHook::_GetClipboardData WinApiHook::get_clipboard_data_ = nullptr;
WinApiHook::_OleSetClipboard WinApiHook::set_ole_clipboard_ = nullptr;
WinApiHook::_OleGetClipboard WinApiHook::get_ole_clipboard_ = nullptr;

HMODULE WinApiHook::kernel32_ = nullptr;
HMODULE WinApiHook::comdlg_ = nullptr;
HMODULE WinApiHook::user32_ = nullptr;
HMODULE WinApiHook::ole32_ = nullptr;

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
		OutputDebugString(L"HookCreateFile failed!");
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

	FreeLibrary(kernel32_);
	return !(kernel32_ = nullptr);
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
		OutputDebugString(L"HookSaveFileAs failed!");
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


	FreeLibrary(comdlg_);
	return !(comdlg_ = nullptr);
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

	FreeLibrary(user32_);
	return !(user32_ = nullptr);
}

bool WinApiHook::HookGetClipboardData()
{
	user32_ = LoadLibrary(L"user32.dll");
	if (user32_ == nullptr)
	{
		return false;
	}

	get_clipboard_data_ = (_GetClipboardData)GetProcAddress(user32_, "GetClipboardData");
	if (get_clipboard_data_ == nullptr)
	{
		return false;
	}

	return !!Mhook_SetHook((PVOID*)&get_clipboard_data_, MyGetClipboardData);
}

bool WinApiHook::UnHookGetClipboardData()
{
	if (get_clipboard_data_)
	{
		Mhook_Unhook((PVOID*)&get_clipboard_data_);
	}

	FreeLibrary(user32_);
	return !(user32_ = nullptr);
}

bool WinApiHook::HookOleSetClipboard()
{
	ole32_ = LoadLibrary(L"ole32.dll");
	if (ole32_ == nullptr)
	{
		return false;
	}

	set_ole_clipboard_ = (_OleSetClipboard)GetProcAddress(ole32_, "OleSetClipboard");
	if (set_ole_clipboard_ == nullptr)
	{
		return false;
	}

	return !!Mhook_SetHook((PVOID*)&set_ole_clipboard_, MyOleSetClipboard);
}

bool WinApiHook::UnHookOleSetClipboard()
{
	if (set_ole_clipboard_)
	{
		Mhook_Unhook((PVOID*)&set_ole_clipboard_);
	}

	FreeLibrary(ole32_);

	return !(ole32_ = nullptr);
}

bool WinApiHook::HookOleGetClipboard()
{
	ole32_ = LoadLibrary(L"ole32.dll");
	if (ole32_ == nullptr)
	{
		return false;
	}

	get_ole_clipboard_ = (_OleGetClipboard)GetProcAddress(ole32_, "OleGetClipboard");
	if (get_ole_clipboard_ == nullptr)
	{
		return false;
	}

	return !!Mhook_SetHook((PVOID*)&get_ole_clipboard_, MyOleGetClipboard);
}

bool WinApiHook::UnHookOleGetClipboard()
{
	if (get_ole_clipboard_)
	{
		Mhook_Unhook((PVOID*)&get_ole_clipboard_);
	}

	FreeLibrary(ole32_);

	return !(ole32_ = nullptr);
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
	Tip::Instance()->Show(Tip::FROM_API);

	char szFileFullPath[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//获取文件路径 
	std::string name(szFileFullPath, MAX_PATH);

	return (name.find("wps.exe") == std::string::npos &&
		name.find("et.exe") == std::string::npos &&
		name.find("wpp.exe") == std::string::npos); //wps false
}

BOOL WinApiHook::MyGetSaveFileNameW(LPOPENFILENAME lpofn)
{
	Tip::Instance()->Show(Tip::FROM_API);

	char szFileFullPath[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//获取文件路径 
	std::string name(szFileFullPath, MAX_PATH);

	return (name.find("wps.exe") == std::string::npos &&
		name.find("et.exe") == std::string::npos &&
		name.find("wpp.exe") == std::string::npos); //wps false
}

HANDLE WinApiHook::MySetClipboardData(UINT uFormat, HANDLE hMem)
{
	if (hMem == nullptr)
	{
		return set_clipboard_data_(uFormat, hMem);
	}


	//size_t size = GlobalSize(hMem);
	//if (size > len_limit_copy)
	{
		if (uFormat == CF_TEXT)
		{
			char* src = (char*)GlobalLock(hMem);
			int len = strlen(src);
			if (src != nullptr && len > len_limit_copy)
			{
				for (int i = len_limit_copy ; i < len; i++)
				{
					src[i] = 0;
				}

			}
			GlobalUnlock(hMem);

		}
		else if (uFormat == CF_UNICODETEXT)
		{
			wchar_t* src = (wchar_t*)GlobalLock(hMem);
			int len = wcslen(src);

			if (src != nullptr && len > len_limit_copy)
			{
				for (int i = len_limit_copy; i < len; i++)
				{
					src[i] = 0;
				}
			}
			GlobalUnlock(hMem);

		}
		else if (uFormat == CF_OEMTEXT)
		{

		}
	}

	return set_clipboard_data_(uFormat, hMem);;
}

HANDLE WinApiHook::MyGetClipboardData(_In_ UINT uFormat)
{
	return get_clipboard_data_(uFormat);
}

HRESULT WinApiHook::MyOleSetClipboard(LPDATAOBJECT pDataObj)
{
	char szFileFullPath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//获取文件路径 
	std::string name(szFileFullPath, MAX_PATH);

	if (name.find("et.exe") == std::string::npos) //wps false
		return S_OK;
	else
		return set_ole_clipboard_(nullptr);
}

HRESULT WinApiHook::MyOleGetClipboard(LPDATAOBJECT * ppDataObj)
{
	HRESULT hr = get_ole_clipboard_(ppDataObj);

	if (ppDataObj == nullptr || *ppDataObj == nullptr)
		return hr;

	{
		FORMATETC formatetcIn{ CF_TEXT , nullptr, 1, -1, 1 };
		STGMEDIUM medium;
		if (S_OK == (*ppDataObj)->GetData(&formatetcIn, &medium))
		{
			char* src = (char*)GlobalLock(medium.hGlobal);
			int len = strlen(src);
			if (src != nullptr && len > len_limit_copy)
			{
				for (int i = len_limit_copy; i < len; i++)
				{
					src[i] = 0;
				}

			}
			GlobalUnlock(medium.hGlobal);
		}
	}

	{
		FORMATETC formatetcIn{ CF_UNICODETEXT , nullptr, 1, -1, 1 };
		STGMEDIUM medium;
		if (S_OK == (*ppDataObj)->GetData(&formatetcIn, &medium))
		{
			wchar_t* src = (wchar_t*)GlobalLock(medium.hGlobal);
			int len = wcslen(src);
			if (src != nullptr && len > len_limit_copy)
			{
				for (int i = len_limit_copy; i < len; i++)
				{
					src[i] = 0;
				}

			}
			GlobalUnlock(medium.hGlobal);
		}
	}

	return hr;
}
