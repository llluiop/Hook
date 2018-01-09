#ifndef __dll_loader_h__
#define __dll_loader_h__

#ifdef DLL_LOADER_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

//************************************
// Method:    InstallHook
// FullName:  InstallHook
// Access:    public 
// Returns:   extern " DLL_EXPORT bool
// Qualifier: install target_dll to the process of pid
// Parameter: DWORD pid
// Parameter: char * target_dll
//************************************
extern "C" DLL_EXPORT bool InstallHook(DWORD pid, const char* target_dll);

//************************************
// Method:    WaitForDllLoaded
// FullName:  WaitForDllLoaded
// Access:    public 
// Returns:   extern " DLL_EXPORT bool
// Qualifier: wait for target_dll loaded into the process of pid 
// Parameter: pid
//************************************
extern "C" DLL_EXPORT bool WaitForDllLoaded(DWORD pid);

//************************************
// Method:    UnInstallHook
// FullName:  UnInstallHook
// Access:    public 
// Returns:   extern " DLL_EXPORT bool
// Qualifier: uninstall 
//************************************
extern "C" DLL_EXPORT bool UnInstallHook();


#endif


