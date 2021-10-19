cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  lodepng
  GIT_REPOSITORY "https://github.com/lvandeve/lodepng.git"
  GIT_TAG 8c6a9e30576f07bf470ad6f09458a2dcd7a6a84a
)

FetchContent_GetProperties ( lodepng )

if ( NOT lodepng_POPULATED )
  FetchContent_Populate( lodepng )
  message( STATUS "lodepng source dir: ${lodepng_SOURCE_DIR}" )
  message( STATUS "lodepng binary dir: ${lodepng_BINARY_DIR}" )

  set( LODEPNG_SOURCES
    ${lodepng_SOURCE_DIR}/lodepng.cpp
  )
endif()
