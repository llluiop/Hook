#include "hook.h"
#include "winapi.h"
#include "wincom.h"

void hook()
{
	WinApiHook::HookSaveFileAs();
	WinComHook::HookSaveFileAs();
	WinApiHook::HookSetClipboardData();
	WinApiHook::HookGetClipboardData();
	WinApiHook::HookOleSetClipboard();
	WinApiHook::HookOleGetClipboard();
}

void unhook()
{
	WinApiHook::UnHookSaveFileAs();
	WinComHook::UnHookSaveFileAs();
	WinApiHook::UnHookSetClipboardData();
	WinApiHook::UnHookGetClipboardData();
	WinApiHook::UnHookOleSetClipboard();
	WinApiHook::UnHookOleGetClipboard();
}
