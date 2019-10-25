cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_helpers_sources LANGUAGES CXX )

set ( ENGINE_HELPERS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/fileBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/fileBuffer.h
  
  ${ENGINE_GFX_SHADERS_SOURCES}

  PARENT_SCOPE
)
