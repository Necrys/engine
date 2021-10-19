cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  glregistry
  GIT_REPOSITORY git@github.com:KhronosGroup/OpenGL-Registry.git
  GIT_TAG 6eca3c2cdc3d1a1147302052e1f1f61cc487b79b
)

FetchContent_GetProperties( glregistry )

if ( NOT glregistry_POPULATED )
  FetchContent_Populate( glregistry )
  message( STATUS "GL Registry source dir: ${glregistry_SOURCE_DIR}" )
  message( STATUS "GL Registry binary dir: ${glregistry_BINARY_DIR}" )
endif()
