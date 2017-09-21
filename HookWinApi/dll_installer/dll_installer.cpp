// dll_installer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <ShlObj.h>
#include <Windows.h>
#include "../dll_loader/dll_loader.h"

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#pragma comment(lib, "dll_loader.lib")



int main(int argc, char* argv[])
{
	if (argc <= 1 || argv[1] == nullptr)
	{
		return -1;
	}

	InstallHook(atoi(argv[1]), "C:\\Users\\liuyu\\Source\\Repos\\Hook\\HookWinApi\\Debug\\sync_dll.dll");

    WaitForDllLoaded(atoi(argv[1]));

	return UnInstallHook();
}

