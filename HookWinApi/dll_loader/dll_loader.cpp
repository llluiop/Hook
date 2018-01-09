// dll_loader.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "dll_loader.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <mutex>
#include <future>
#include <vector>
#include <oledb.h>
#include <Sddl.h>
#include <Aclapi.h>

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


bool SetObjectToLowIntegrity(
	HANDLE hObject, SE_OBJECT_TYPE type = SE_KERNEL_OBJECT)
{

	LPCWSTR LOW_INTEGRITY_SDDL_SACL = L"S:(ML;;NW;;;LW)";
	bool bRet = false;
	DWORD dwErr = ERROR_SUCCESS;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pSacl = NULL;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;

	if (ConvertStringSecurityDescriptorToSecurityDescriptor(
		LOW_INTEGRITY_SDDL_SACL, SDDL_REVISION_1, &pSD, NULL))
	{
		if (GetSecurityDescriptorSacl(
			pSD, &fSaclPresent, &pSacl, &fSaclDefaulted))
		{
			dwErr = SetSecurityInfo(
				hObject, type, LABEL_SECURITY_INFORMATION,
				NULL, NULL, NULL, pSacl);

			bRet = (ERROR_SUCCESS == dwErr);
		}

		LocalFree(pSD);
	}

	return bRet;
}

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
		::LoadLibraryA(g_dll);
		OpenEventA(EVENT_MODIFY_STATE, FALSE, g_sig);

	});
	return ::CallNextHookEx(g_hook, nCode, wParam, lParam);
}

bool MakeEventName(DWORD pid, const char* path)
{
	char name[64] = { 0 };
	_splitpath_s(path, nullptr, 0, nullptr, 0, name, 64, nullptr, 0);
	sprintf_s(g_sig, "Global\\%d-%s", pid, name);

	return true;
}

bool InstallHook(DWORD pid, const char* target_dll)
{
	strncpy_s(g_dll, target_dll, MAX_PATH);

	MakeEventName(pid, target_dll);

	SECURITY_DESCRIPTOR secutityDese;
	::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
	::SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE);
	SECURITY_ATTRIBUTES securityAttr;
	// set SECURITY_ATTRIBUTES
	securityAttr.nLength = sizeof SECURITY_ATTRIBUTES;
	securityAttr.bInheritHandle = FALSE;
	securityAttr.lpSecurityDescriptor = &secutityDese;


	g_event = CreateEventA(&securityAttr, FALSE, FALSE, g_sig);
	SetObjectToLowIntegrity(g_event);
	if (NULL == g_event)
	{
		DWORD err = GetLastError();
		return false;
	}

	auto tids = GetThreadIDByProcssID(pid);
	for (auto tid : tids)
	{
		g_hook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)GetMsgProc, g_hModule, tid);
		if (g_hook != nullptr)
		{
			break;
		}
	}
	
	return g_hook != nullptr;
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