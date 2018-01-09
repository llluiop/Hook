// dll_installer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <ShlObj.h>
#include <Windows.h>
#include <iostream>
#include "../dll_loader/dll_loader.h"

#ifndef _TEST
	#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#endif

#pragma comment(lib, "dll_loader.lib")

std::string get_path()
{
	char cWinDir[MAX_PATH] = { 0 };
	GetModuleFileNameA(nullptr, cWinDir, MAX_PATH);
	(strrchr(cWinDir, ('\\')))[1] = 0;

	std::string path(cWinDir);
	path.append("sync_dll.dll");

	return path;
}

int main(int argc, char* argv[])
{
	if (argc <= 2 || argv[1] == nullptr)
	{
		return -1;
	}


	DWORD target_process_id = atoi(argv[1]);
	std::string target_process_name(argv[2]);

#ifdef _TEST
	GetWindowThreadProcessId(FindWindowA("OpusApp", "新建 Microsoft Word 文档.docx - Word"), &target_process_id);
	target_process_id = 10572;
	//std::cin >> target_process;
#endif // 

	auto dll_path = get_path();
	if (!InstallHook(target_process_id, dll_path.c_str()))
		return false;

    WaitForDllLoaded(target_process_id);

	return UnInstallHook();
}

