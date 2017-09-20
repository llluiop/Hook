// Syncdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "sync.h"
#include <TlHelp32.h>
#include <Windows.h>

HHOOK g_hook;
extern HMODULE g_hModule;

DWORD GetThreadIDByProcssID(DWORD dwProcessID)
{
	DWORD dwThreadID = 0;
	THREADENTRY32 te32 = { sizeof(te32) };
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if (Thread32First(hThreadSnap, &te32))
	{
		do {
			if (dwProcessID == te32.th32OwnerProcessID)
			{
				dwThreadID = te32.th32ThreadID;
				break;
			}
		} while (Thread32Next(hThreadSnap, &te32));
	}

	return dwThreadID;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return ::CallNextHookEx(g_hook, nCode, wParam, lParam);
}


bool InstallHook(DWORD pid)
{
	DWORD tid = GetThreadIDByProcssID(pid);
	g_hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hModule, tid);
	if (g_hook == nullptr)
	{
		DWORD err = GetLastError();
		return false;
	}
	MessageBox(nullptr, L"hook suc!", nullptr, MB_OK);

	return true;
}


bool UnInstallHook()
{
	return !!UnhookWindowsHookEx(g_hook);
}

