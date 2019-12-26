cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  glregistry
  GIT_REPOSITORY https://github.com/KhronosGroup/OpenGL-Registry.git
)

FetchContent_GetProperties( glregistry )

if ( NOT glregistry_POPULATED )
  FetchContent_Populate( glregistry )
  message( STATUS "GL Registry source dir: ${glregistry_SOURCE_DIR}" )
  message( STATUS "GL Registry binary dir: ${glregistry_BINARY_DIR}" )
endif()
