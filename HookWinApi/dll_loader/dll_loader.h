#ifndef __dll_loader_h__
#define __dll_loader_h__

#ifdef DLL_LOADER_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" DLL_EXPORT bool InstallHook(DWORD pid, char* target_dll);
//************************************
// Method:    WaitForDllLoaded
// FullName:  WaitForDllLoaded
// Access:    public 
// Returns:   extern " DLL_EXPORT bool
// Qualifier: wait for target_dll loaded by process of pid
// Parameter: pid
//************************************
extern "C" DLL_EXPORT bool WaitForDllLoaded(DWORD pid);
extern "C" DLL_EXPORT bool UnInstallHook();


#endif


