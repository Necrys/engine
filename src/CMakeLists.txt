cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_sources LANGUAGES CXX )

add_subdirectory ( core )
add_subdirectory ( gfx )
add_subdirectory ( helpers )

set ( ENGINE_SOURCES
  ${ENGINE_CORE_SOURCES}
  ${ENGINE_GFX_SOURCES}
  ${ENGINE_HELPERS_SOURCES}

  PARENT_SCOPE
)
