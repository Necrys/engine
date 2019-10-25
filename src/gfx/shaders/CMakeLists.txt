cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_gfx_shaders_sources LANGUAGES CXX )

set ( ENGINE_GFX_SHADERS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/batch_renderer_fs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/batch_renderer_vs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/gui_renderer_fs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/gui_renderer_vs.h

  PARENT_SCOPE
)
