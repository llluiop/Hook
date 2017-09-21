#ifndef __sync_h__
#define __sync_h__

#ifdef SYNCDLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" DLL_EXPORT bool Hook(DWORD pid);



#endif
