cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  eglregistry
  GIT_REPOSITORY https://github.com/KhronosGroup/EGL-Registry.git
)

FetchContent_GetProperties( eglregistry )

if ( NOT eglregistry_POPULATED )
  FetchContent_Populate( eglregistry )
  message( STATUS "EGL Registry source dir: ${eglregistry_SOURCE_DIR}" )
  message( STATUS "EGL Registry binary dir: ${eglregistry_BINARY_DIR}" )
endif()
