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


HRESULT CreateQSecDescriptor(
)
{


	HANDLE hToken = NULL;
	DWORD dwBufferSize = 0;
	PTOKEN_USER pTokenUser = NULL;
	PSID pEveryoneSid = NULL;
	PSID pTrustedUserSid = NULL;
	PACL pDacl = NULL;
	DWORD cbDacl = 0;
	SECURITY_DESCRIPTOR sd;
	DWORD dwErrorCode = 0;
	HRESULT hr = S_FALSE;

	// Open the access token associated with the calling process.  
	if (OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_QUERY,
		&hToken
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"OpenProcessToken failed. GetLastError returned: %d\n", dwErrorCode);
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	// Retrieve the token information in a TOKEN_USER structure.  
	GetTokenInformation(
		hToken,
		TokenUser,      // Request for a TOKEN_USER structure.  
		NULL,
		0,
		&dwBufferSize
	);

	pTokenUser = (PTOKEN_USER) new BYTE[dwBufferSize];
	memset(pTokenUser, 0, dwBufferSize);
	if (GetTokenInformation(hToken,
		TokenUser,
		pTokenUser,
		dwBufferSize,
		&dwBufferSize
	))
	{
		CloseHandle(hToken);
	}
	else
	{
		dwErrorCode = GetLastError();
		wprintf(L"GetTokenInformation failed. GetLastError returned: %d\n", dwErrorCode);
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	if (IsValidSid(pTokenUser->User.Sid) == FALSE)
	{
		wprintf(L"The owner SID is invalid.\n");
		delete[] pTokenUser;
		return S_FALSE;
	}

	// Retrieve the SID of the Everyone group.  
	SID_IDENTIFIER_AUTHORITY WorldAuth = SECURITY_WORLD_SID_AUTHORITY;
	if (AllocateAndInitializeSid(
		&WorldAuth,          // Top-level SID authority  
		1,                   // Number of subauthorities  
		SECURITY_WORLD_RID,  // Subauthority value  
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		&pEveryoneSid        // SID returned as OUT parameter  
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"AllocateAndInitializeSid failed. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	// Retrieve the SID of the trusted user.  
	//hr = GetSid(wszUserName, &pTrustedUserSid);
	//if (FAILED(hr))
	//{
	//	wprintf(L"GetSid failed. Error code: 0x%X\n", hr);
	//	delete[] pTokenUser;
	//	FreeSid(pEveryoneSid);
	//	return hr;
	//}

	// Calculate the amount of memory that must be allocated for the DACL.  
	cbDacl = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) * 2 - sizeof(DWORD) * 2;
	cbDacl += GetLengthSid(pTokenUser->User.Sid);
	cbDacl += GetLengthSid(pEveryoneSid);
	//cbDacl += GetLengthSid(pTrustedUserSid);

	// Create and initialize an ACL.  
	pDacl = (PACL) new BYTE[cbDacl];
	if (pDacl == NULL)
	{
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		//FreeSid(pTrustedUserSid);
		return S_FALSE;
	}
	memset(pDacl, 0, cbDacl);
	if (InitializeAcl(
		pDacl,
		cbDacl,
		ACL_REVISION  // Required constant  
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"InitializeAcl failed. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		//FreeSid(pTrustedUserSid);
		delete[] pDacl;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	// Add access-allowed ACEs for the three trustees.  
	if (AddAccessAllowedAce(
		pDacl,                    // Pointer to the ACL.  
		ACL_REVISION,             // Required constant  
		GENERIC_ALL,  // Access mask  
		pTokenUser->User.Sid      // Pointer to the trustee's SID  
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"AddAccessAllowedAce failed for the queue owner. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		//FreeSid(pTrustedUserSid);
		delete[] pDacl;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	if (AddAccessAllowedAce(
		pDacl,                     // Pointer to the ACL  
		ACL_REVISION,              // Required constant  
		GENERIC_ALL,  // Access mask  
		pEveryoneSid               // Pointer to the trustee's SID  
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"AddAccessAllowedAce failed for the Everyone group. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		//FreeSid(pTrustedUserSid);
		delete[] pDacl;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	//if (AddAccessAllowedAce(
	//	pDacl,                    // Pointer to the ACL structure  
	//	ACL_REVISION,             // Required constant  
	//	GENERIC_ALL,  // Access mask  
	//	pTrustedUserSid           // Pointer to the trustee's SID  
	//) == FALSE)
	//{
	//	dwErrorCode = GetLastError();
	//	wprintf(L"AddAccessAllowedAce failed for the trusted owner. GetLastError returned: %d\n", dwErrorCode);
	//	delete[] pTokenUser;
	//	FreeSid(pEveryoneSid);
	//	FreeSid(pTrustedUserSid);
	//	delete[] pDacl;
	//	return HRESULT_FROM_WIN32(dwErrorCode);
	//}

	// Initialize an absolute SECURITY_DESCRIPTOR structure.  
	if (InitializeSecurityDescriptor(
		&sd,
		SECURITY_DESCRIPTOR_REVISION  // Required constant  
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"InitializeSecurityDescriptor failed. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		FreeSid(pTrustedUserSid);
		delete[] pDacl;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	// Insert the DACL into the absolute SECURITY_DESCRIPTOR structure.  
	if (SetSecurityDescriptorDacl(
		&sd,
		TRUE,
		pDacl,
		FALSE
	) == FALSE)
	{
		dwErrorCode = GetLastError();
		wprintf(L"SetSecurityDescriptorDacl failed. GetLastError returned: %d\n", dwErrorCode);
		delete[] pTokenUser;
		FreeSid(pEveryoneSid);
		FreeSid(pTrustedUserSid);
		delete[] pDacl;
		return HRESULT_FROM_WIN32(dwErrorCode);
	}

	SECURITY_ATTRIBUTES securityAttr;
	// set SECURITY_ATTRIBUTES
	securityAttr.nLength = sizeof SECURITY_ATTRIBUTES;
	securityAttr.bInheritHandle = FALSE;
	securityAttr.lpSecurityDescriptor = &sd;

	g_event = CreateEventA(&securityAttr, FALSE, FALSE, g_sig);

	// Create a queue using the absolute SECURITY_DESCRIPTOR structure.  
	//hr = CreateMSMQQueue(
	//	wszPathName,
	//	&sd,
	//	wszOutFormatName,
	//	pdwOutFormatNameLength
	//);
	//if (FAILED(hr))
	//{
	//	wprintf(L"CreateMSMQQueue returned 0x%X\n", hr);
	//}

	delete[] pTokenUser;
	FreeSid(pEveryoneSid);
	//FreeSid(pTrustedUserSid);
	delete[] pDacl;
	return hr;
}

#include <oledb.h>
#include <Sddl.h>
#include <Aclapi.h>
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
		HANDLE h = OpenEventA(EVENT_MODIFY_STATE, FALSE, g_sig);
		//if (h == nullptr)
		//{
			auto d = GetLastError();
		//}
		auto b = SetEvent(h);
		if (b == FALSE)
		{
			auto d = GetLastError();
		}
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
		g_hook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)GetMsgProc, g_hModule, tid);
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
	//CreateQSecDescriptor();
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