// dll_installer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <ShlObj.h>
#include <Windows.h>
#include "../syncdll/sync.h"

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib, "sync_dll.lib")



int main(int argc, char* argv[])
{
	if (argc <= 1 || argv[1] == nullptr)
	{
		return -1;
	}

	//HMODULE injected_dll = LoadLibrary(L"sync_dll.dll");
	//if (injected_dll ==  nullptr)
	//{
	//	return -1;
	//}

	//auto func = GetProcAddress(injected_dll, "sync");
	//int remote_process_id = atoi(argv[1]);
	//int remote_thread_id = GetThreadIDByProcssID(remote_process_id);
	
	if (0 == strcmp(argv[1], "-install"))
	{
		MessageBox(nullptr, L"install!", nullptr, MB_OK);
		InstallHook(atoi(argv[2]));
	}
	else if (0 == strcmp(argv[1], "-uninstall"))
	{
		MessageBox(nullptr, L"uninstall!", nullptr, MB_OK);
		UnInstallHook();
	}

    return 0;
}

