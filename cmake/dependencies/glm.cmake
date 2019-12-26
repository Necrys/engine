cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG        "0.9.9.5"
)

FetchContent_GetProperties( glm )

if ( NOT glm_POPULATED )
  FetchContent_Populate( glm )
  message( STATUS "GLM source dir: ${glm_SOURCE_DIR}" )
  message( STATUS "GLM binary dir: ${glm_BINARY_DIR}" )
endif()
