#include "hook.h"
#include "winapi.h"
#include "wincom.h"

void hook()
{
	WinApiHook::HookSaveFileAs();
	WinComHook::HookSaveFileAs();
	WinApiHook::HookSetClipboardData();
}

void unhook()
{
	WinApiHook::UnHookSaveFileAs();
	WinComHook::UnHookSaveFileAs();
	WinApiHook::UnHookSetClipboardData();
}
