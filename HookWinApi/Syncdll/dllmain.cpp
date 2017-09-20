// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "./hook/winapi.h"

HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hModule = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"process attach");
		MessageBox(nullptr, L"HOOK!", nullptr, MB_OK);
		WinApiHook::HookCreateFile();
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(L"thread attach");
		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(L"process detach");
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(L"process detach");
		MessageBox(nullptr, L"UNHOOK!", nullptr, MB_OK);
		break;
	}
	return TRUE;
}

