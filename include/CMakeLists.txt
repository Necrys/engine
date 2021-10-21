cmake_minimum_required ( VERSION 3.12.1 )
project ( engine_headers LANGUAGES CXX )

set ( ENGINE_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/application.h
  ${CMAKE_CURRENT_SOURCE_DIR}/batchrenderer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/device.h
  ${CMAKE_CURRENT_SOURCE_DIR}/engine.h
  ${CMAKE_CURRENT_SOURCE_DIR}/glapi.h
  ${CMAKE_CURRENT_SOURCE_DIR}/guirenderer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/input.h
  ${CMAKE_CURRENT_SOURCE_DIR}/keys.h
  ${CMAKE_CURRENT_SOURCE_DIR}/logger.h
  ${CMAKE_CURRENT_SOURCE_DIR}/logger.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
  ${CMAKE_CURRENT_SOURCE_DIR}/texture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/window.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sprite.h

  PARENT_SCOPE
)
