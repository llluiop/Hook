// Syncdll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "sync.h"
#include <Windows.h>
#include "./hook/winapi.h"


bool Hook(DWORD pid)
{
	OutputDebugString(L"hook");
	MessageBox(nullptr, L"HOOK!", nullptr, MB_OK);
	return WinApiHook::HookCreateFile();
}
