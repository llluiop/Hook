// dll_loader.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "dll_loader.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <mutex>
#include <future>
#include <vector>

#pragma data_seg(".Shared")
//HANDLE g_sig = nullptr;
char g_sig[512] = { 0 };
char g_dll[260] = {0};
#pragma data_seg()

HHOOK g_hook;
std::once_flag g_flag;
extern HMODULE g_hModule;

#pragma comment(linker,"/SECTION:.Shared,RWS")

using namespace std;

vector<DWORD> GetThreadIDByProcssID(DWORD pid)
{
	vector<DWORD> tids;
	THREADENTRY32 te32 = { sizeof(te32) };
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	if (Thread32First(hThreadSnap, &te32))
	{
		do {
			if (pid == te32.th32OwnerProcessID)
			{
				tids.push_back(te32.th32ThreadID);
			}
		} while (Thread32Next(hThreadSnap, &te32));
	}

	return std::move(tids);
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	std::call_once(g_flag, [] {
		HMODULE module = ::LoadLibraryA(g_dll);

		auto func = GetProcAddress(module, "Hook");
		std::async(std::launch::async, [&] {
			if(func) func();
			SetEvent(OpenEventA(SYNCHRONIZE, FALSE, g_sig));
		});

	});
	return ::CallNextHookEx(g_hook, nCode, wParam, lParam);
}


bool InstallHook(DWORD pid, char* target_dll)
{
	strncpy_s(g_dll, target_dll, 260);
	sprintf_s(g_sig, "%d-%s", pid, target_dll);

	auto tids = GetThreadIDByProcssID(pid);
	for (auto tid : tids)
	{
		g_hook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)CallWndProc, g_hModule, tid);
		if (g_hook != nullptr)
		{
			break;
		}
	}
	
	if (g_hook == nullptr)
	{
		DWORD err = GetLastError();
		return false;
	}

	if (NULL == CreateEventA(nullptr, FALSE, FALSE, g_sig))
	{
		return false;
	}

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