// dll_installer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <ShlObj.h>
#include <Windows.h>
#include <iostream>
#include "../dll_loader/dll_loader.h"

#ifndef _DEBUG
	#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#endif

#pragma comment(lib, "dll_loader.lib")



int main(int argc, char* argv[])
{
	if (argc <= 1 || argv[1] == nullptr)
	{
		return -1;
	}

	DWORD target_process = atoi(argv[1]);
#ifdef _DEBUG
	GetWindowThreadProcessId(FindWindowA("OpusApp", "新建 Microsoft Word 文档.docx - Word"), &target_process);
	target_process = 10572;
	//std::cin >> target_process;
#endif // DEBUG

#ifdef _WIN64
	InstallHook(target_process, "C:\\Users\\liuyu\\Source\\Repos\\Hook\\HookWinApi\\x64\\Debug\\sync_dll.dll");
#else
	InstallHook(target_process, "C:\\Users\\liuyu\\Source\\Repos\\Hook\\HookWinApi\\Debug\\sync_dll.dll");
#endif // _WIN64


    WaitForDllLoaded(target_process);

	return UnInstallHook();
}

