cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG        "v1.69"
)

FetchContent_GetProperties( imgui )

if ( NOT imgui_POPULATED )
  FetchContent_Populate( imgui )
  message( STATUS "ImGui source dir: ${imgui_SOURCE_DIR}" )
  message( STATUS "ImGui binary dir: ${imgui_BINARY_DIR}" )

  set ( IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  )
endif()
