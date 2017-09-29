#include "wincom.h"
#include "mhook-lib/mhook.h"
#include "asm/cocreateinstance.h"


IFileDialog* WinComHook::iFileDialog_ = nullptr;
WinComHook::_Show WinComHook::show_ = nullptr;

HRESULT WinComHook::MyShow(IFileDialog * pIFileDialog, HWND hwndOwner)
{
	return S_FALSE;
}
#pragma pack(push, 1)
struct Thunk
{
	BYTE jump;
	DWORD real;
};
#pragma pack(pop)

#ifndef _WIN64
__declspec(naked) void Calc()
{
	__asm
	{
		push ebx
		mov  ebx, dword ptr [CoCreateInstance]
		add  ebx, 5
		mov  eax, ebx
		pop  ebx
		ret
	}
}

__declspec(naked) HRESULT STDAPICALLTYPE
PureCoCreateInstance(
	_In_ REFCLSID rclsid,
	_In_opt_ LPUNKNOWN pUnkOuter,
	_In_ DWORD dwClsContext,
	_In_ REFIID riid,
	_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR * ppv
)
{
		__asm
		{
			mov  edi, edi
			push ebp
			mov  ebp, esp
			call Calc
			jmp  eax
		}
}
#endif

template<class T>
bool pure(const T addr)
{
#ifdef _WIN64
	auto code = (SHORT)addr;
#else
	auto code = (char)*(int*)addr;
	return code == 0x8b; //8bff mov edi, edi
#endif

	return true;
}
bool WinComHook::HookSaveFileAs()
{
	return true;
	auto h = CoInitialize(nullptr); return true;
	HRESULT hr;
	auto t = CoCreateInstance;
	if (!pure(CoCreateInstance))
	{
		hr = PureCoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&iFileDialog_));

	}
	else
	{
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&iFileDialog_));
	}

	if (!SUCCEEDED(hr))
	{
		return false;
	}

	show_ = (_Show)*((DWORD*)*(DWORD*)iFileDialog_ + 3);
	if (show_)
	{
		//return !!Mhook_SetHook((PVOID*)&show_, MyShow);
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
