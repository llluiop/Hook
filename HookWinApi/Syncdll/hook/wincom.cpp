#include "wincom.h"
#include "mhook-lib/mhook.h"
#include "asm/cocreateinstance.h"
#include "../sweeper/sweeper.h"
#include "../tip/tip.h"
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
		Tip::Instance()->Show(Tip::FROM_COM);
		Sweeper::Instance()->SaveAsTriggered();
		return S_OK;
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


template<class T>
bool pure(const T addr)
{

	auto code = (char)*(int*)addr;
	return code == 0x8b; //8bff mov edi, edi


	return true;
}
#endif


bool WinComHook::HookSaveFileAs()
{
	if (!SUCCEEDED(CoInitialize(nullptr)))
	{
		return false;
	}

	HRESULT hr;

#ifndef _WIN64
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
#else
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&iFileDialog_));
#endif

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
