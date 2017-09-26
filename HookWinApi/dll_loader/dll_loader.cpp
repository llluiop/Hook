// dll_loader.cpp : 定义 DLL 应用程序的导出函数。
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
char g_sig[MAX_PATH] = { 0 };
char g_dll[MAX_PATH] = {0};
#pragma data_seg()

HHOOK g_hook;
HANDLE g_event;
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

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	std::call_once(g_flag, [] {
		HMODULE module = ::LoadLibraryA(g_dll);

		auto func = GetProcAddress(module, "Hook");
		std::async(std::launch::async, [&] {
			if(func) func();

			SetEvent(OpenEventA(EVENT_MODIFY_STATE, FALSE, g_sig));
		});

	});
	return ::CallNextHookEx(g_hook, nCode, wParam, lParam);
}

bool MakeEventName(DWORD pid, char* path)
{
	char name[64] = { 0 };
	_splitpath_s(path, nullptr, 0, nullptr, 0, name, 64, nullptr, 0);
	sprintf_s(g_sig, "Global\\%d-%s", pid, name);

	return true;
}

bool InstallHook(DWORD pid, char* target_dll)
{
	strncpy_s(g_dll, target_dll, MAX_PATH);

	auto tids = GetThreadIDByProcssID(pid);
	for (auto tid : tids)
	{
		g_hook = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hModule, tid);
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

	MakeEventName(pid, target_dll);
	g_event = CreateEventA(nullptr, FALSE, FALSE, g_sig);

	if (NULL == g_event)
	{
		DWORD err = GetLastError();
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

	HANDLE events[2] = {g_event, target_process};
	return WAIT_OBJECT_0 ==  WaitForMultipleObjects(2, events, FALSE, INFINITE);
}

bool UnInstallHook()
{
	return !!UnhookWindowsHookEx(g_hook);
}