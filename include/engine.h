#pragma once

#ifdef ENGINE_LIB_SHARED
  #ifdef BUILD_DLL
    #define ENGINE_LIB_API __declspec(dllexport)
  #else//BUILD_DLL
    #define ENGINE_LIB_API __declspec(dllimport)
  #endif//BUILD_DLL
#else//ENGINE_LIB_API
  #define ENGINE_LIB_API
#endif//ENGINE_LIB_API

#include <application.h>
