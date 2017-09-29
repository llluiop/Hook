#pragma once

#include <Windows.h>
#include <Shobjidl.h>


class WinComHook
{
public:
	typedef HRESULT(STDMETHODCALLTYPE* _Show)(
		IFileDialog* pIFileDialog,
		_In_opt_  HWND hwndOwner);

	static HRESULT STDMETHODCALLTYPE MyShow(
		IFileDialog* pIFileDialog,
		_In_opt_  HWND hwndOwner);


	typedef HRESULT(STDAPICALLTYPE*
		_CoCreateInstance)(
			_In_ REFCLSID rclsid,
			_In_opt_ LPUNKNOWN pUnkOuter,
			_In_ DWORD dwClsContext,
			_In_ REFIID riid,
			_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR * ppv);

	static HRESULT STDAPICALLTYPE
		MyCoCreateInstance(
			_In_ REFCLSID rclsid,
			_In_opt_ LPUNKNOWN pUnkOuter,
			_In_ DWORD dwClsContext,
			_In_ REFIID riid,
			_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR * ppv);

	static bool HookSaveFileAs();
	static bool UnHookSaveFileAs();

	static bool HookCoCreateInstance();
	static bool UnHookCoCreateInstance();

private:
	static IFileDialog* iFileDialog_;
	static _Show show_;
	static _CoCreateInstance cocreateinstance_;
};
