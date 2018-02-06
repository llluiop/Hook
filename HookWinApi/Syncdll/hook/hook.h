#ifndef __hook_h__
#define __hook_h__

#ifdef SYNCDLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


extern "C" DLL_EXPORT void hook();


extern "C" DLL_EXPORT void unhook();




#endif