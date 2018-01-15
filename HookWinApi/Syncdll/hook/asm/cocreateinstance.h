#pragma once

#include <Windows.h>
#include <Shobjidl.h>

extern "C"
{
#ifndef _WIN64
	HRESULT STDAPICALLTYPE
		PureCoCreateInstance(
			_In_ REFCLSID rclsid,
			_In_opt_ LPUNKNOWN pUnkOuter,
			_In_ DWORD dwClsContext,
			_In_ REFIID riid,
			_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR * ppv,
			_In_ DWORD dwAddr
		);
#else
	HRESULT PureCoCreateInstance(
		_In_ REFCLSID rclsid,
		_In_opt_ LPUNKNOWN pUnkOuter,
		_In_ DWORD dwClsContext,
		_In_ REFIID riid,
		_COM_Outptr_ _At_(*ppv, _Post_readable_size_(_Inexpressible_(varies))) LPVOID FAR * ppv
	);
#endif // !_WIN64
}