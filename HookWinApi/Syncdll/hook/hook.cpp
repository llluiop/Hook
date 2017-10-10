#include "hook.h"
#include "winapi.h"
#include "wincom.h"

void hook()
{
	WinApiHook::HookSaveFileAs();
	WinComHook::HookSaveFileAs();
}

void unhook()
{
	WinApiHook::UnHookSaveFileAs();
	WinComHook::UnHookSaveFileAs();
}
