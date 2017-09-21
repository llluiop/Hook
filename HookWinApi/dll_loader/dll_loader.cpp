// dll_loader.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "dll_loader.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <mutex>
#include <future>

#pragma data_seg(".Shared")
HANDLE g_sig = nullptr;
char g_dll[260] = {0};
#pragma data_seg()

HHOOK g_hook;
std::once_flag g_flag;
extern HMODULE g_hModule;

#pragma comment(linker,"/SECTION:.Shared,RWS")

DWORD GetThreadIDByProcssID(DWORD pid)
{
	DWORD tid = 0;
	THREADENTRY32 te32 = { sizeof(te32) };
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	if (Thread32First(hThreadSnap, &te32))
	{
		do {
			if (pid == te32.th32OwnerProcessID)
			{
				tid = te32.th32ThreadID;
				break;
			}
		} while (Thread32Next(hThreadSnap, &te32));
	}

	return tid;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	std::call_once(g_flag, [] {
		HMODULE module = ::LoadLibraryA(g_dll);

		typedef bool(*Hook)();
		Hook func = (Hook)GetProcAddress(module, "Hook");
		std::async(std::launch::async, [&] {
			if(func) func();
			SetEvent(g_sig);
		});

	});
	return ::CallNextHookEx(g_hook, nCode, wParam, lParam);
}


bool InstallHook(DWORD pid, char* target_dll)
{
	strncpy_s(g_dll, target_dll, 260);

	DWORD tid = GetThreadIDByProcssID(pid);
	if (pid == 0)
	{
		return false;
	}
	
	g_hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hModule, tid);
	if (g_hook == nullptr)
	{
		DWORD err = GetLastError();
		return false;
	}

	MessageBox(nullptr, L"install hook suc!", nullptr, MB_OK);
	g_sig = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return true;
}


bool WaitForDllLoaded(DWORD pid)
{
	//waiting for run
	HANDLE target_process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	if (target_process == nullptr)
	{
		return false;
	}

	HANDLE events[2] = {g_sig, target_process};
	return WAIT_OBJECT_0 ==  WaitForMultipleObjects(2, events, FALSE, INFINITE);
}

bool UnInstallHook()
{
	return !!UnhookWindowsHookEx(g_hook);
}