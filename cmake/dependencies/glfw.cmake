cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG        "3.2.1"
)

FetchContent_GetProperties( glfw )

if ( NOT glfw_POPULATED )
  FetchContent_Populate( glfw )
  message( STATUS "GLFW source dir: ${glfw_SOURCE_DIR}" )
  message( STATUS "GLFW binary dir: ${glfw_BINARY_DIR}" )
  set( GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" )
  set( GLFW_BUILD_TESTS OFF CACHE INTERNAL "" )
  set( GLFW_BUILD_DOCS OFF CACHE INTERNAL "" )
  add_subdirectory( ${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} ) #can be case insensitive
endif()
