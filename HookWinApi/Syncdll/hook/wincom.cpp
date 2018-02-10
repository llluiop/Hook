#include "wincom.h"
#include "mhook-lib/mhook.h"
#include "asm/cocreateinstance.h"
#include <typeinfo>

IFileDialog* WinComHook::iFileDialog_ = nullptr;
WinComHook::_Show WinComHook::show_ = nullptr;

HRESULT WinComHook::MyShow(IFileDialog * pIFileDialog, HWND hwndOwner)
{
	IFileSaveDialog* pCustom = nullptr;
	auto hr = pIFileDialog->QueryInterface(IID_IFileSaveDialog,
		reinterpret_cast<void**>(&pCustom));
	if (SUCCEEDED(hr))
	{
		MessageBox(hwndOwner, L"加密文件不允许此操作", L"文件防泄漏", NULL);
		return S_FALSE;
	}

	return show_(pIFileDialog, hwndOwner);
}


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

#else
	auto code = (char)*(int*)addr;
	return code == 0x8b; //8bff mov edi, edi
#endif

	return true;
}
bool WinComHook::HookSaveFileAs()
{
	if (!SUCCEEDED(CoInitialize(nullptr)))
	{
		return false;
	}

	HRESULT hr;
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
#ifdef _DEBUG
		MessageBox(0,L"CoCreateInstance failed!",0,0);
#endif
		return false;
	}


	show_ = (_Show)*((DWORD_PTR*)*(DWORD_PTR*)iFileDialog_ + 3);
	return !!Mhook_SetHook((PVOID*)&show_, MyShow);
}

bool WinComHook::UnHookSaveFileAs()
{
	if (show_)
	{
		Mhook_Unhook((PVOID*)&show_);
	}

	CoUninitialize();

	return true;
}
