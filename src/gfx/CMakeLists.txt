cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_gfx_sources LANGUAGES CXX )

add_subdirectory ( shaders )

set ( ENGINE_GFX_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/batchrenderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/glapi.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/guirenderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
  
  ${ENGINE_GFX_SHADERS_SOURCES}

  PARENT_SCOPE
)
