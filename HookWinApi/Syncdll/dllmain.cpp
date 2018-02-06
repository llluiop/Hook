// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "./hook/hook.h"
#include <future>


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	     //https://msdn.microsoft.com/en-us/library/windows/desktop/dn633971(v=vs.85).aspx
		//OutputDebugString(L"process attach");
		//std::async(std::launch::async, [] {
		//	hook();
		//});
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(L"thread attach");
		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(L"thread detach");
		break;
	case DLL_PROCESS_DETACH:
		//OutputDebugString(L"process detach");
		unhook();
		break;
	}
	return TRUE;
}

