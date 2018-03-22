#include "hook.h"
#include "winapi.h"
#include "wincom.h"
#include "../sweeper/sweeper.h"

void hook()
{
	Sweeper::Instance()->Start();
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
	Sweeper::Instance()->Stop();
}
