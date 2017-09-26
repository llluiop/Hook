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
	

	static bool HookSaveFileAs();
	static bool UnHookSaveFileAs();

private:
	static IFileDialog* iFileDialog_;
	static _Show show_;
};
