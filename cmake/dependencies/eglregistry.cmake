cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  eglregistry
  GIT_REPOSITORY https://github.com/KhronosGroup/EGL-Registry.git
  GIT_TAG 67e1f42a625289b3f6a7df287e020d78122db32b
)

FetchContent_GetProperties( eglregistry )

if ( NOT eglregistry_POPULATED )
  FetchContent_Populate( eglregistry )
  message( STATUS "EGL Registry source dir: ${eglregistry_SOURCE_DIR}" )
  message( STATUS "EGL Registry binary dir: ${eglregistry_BINARY_DIR}" )
endif()
