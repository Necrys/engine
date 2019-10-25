cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_core_sources LANGUAGES CXX )

set ( ENGINE_CORE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/input.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp

  PARENT_SCOPE
)
