// Syncdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "sync.h"
#include <Windows.h>
#include "./hook/winapi.h"
#include "./hook/wincom.h"


bool Hook(DWORD pid)
{
	OutputDebugString(L"hook");
	MessageBox(nullptr, L"HOOK!", nullptr, MB_OK);
	if(WinApiHook::HookCreateFile())
		OutputDebugString(L"create hook");

	if (WinComHook::HookSaveFileAs())
		OutputDebugString(L"save as hook");

	return true;
}
