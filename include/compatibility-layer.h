#ifndef EASYPACK_COMPATIBILITY_LAYER_H
#define EASYPACK_COMPATIBILITY_LAYER_H

#ifdef _WIN32

#ifdef EXPORT_API
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#else

#define API

#endif


#endif