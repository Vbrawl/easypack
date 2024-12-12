#ifndef EASYPACK_COMPATIBILITY_LAYER_H
#define EASYPACK_COMPATIBILITY_LAYER_H

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>

#ifdef EXPORT_API
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#define access _access
#define chdir _chdir

#define F_OK 0

#else

#define API

#endif


#endif