cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )
project( engine LANGUAGES CXX )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

include( cmake/fetch_dependencies.cmake )

if ( UNIX )
  set ( GL_LIB GL )
elseif ( WIN32 )
  set ( GL_LIB OpenGL32 )
endif ()

option( BUILD_SHARED_LIB "Build shared library" OFF )

if ( BUILD_SHARED_LIB )
  set( INTERNAL_DEFINITIONS
    ${INTERNAL_DEFINITIONS}
    ENGINE_LIB_SHARED
    BUILD_DLL
  )
  set( EXTERNAL_DEFINITIONS
    ${EXTERNAL_DEFINITIONS}
    ENGINE_LIB_SHARED
  )
  set( LIB_TYPE SHARED )
else ()
  set( LIB_TYPE STATIC )
endif ( BUILD_SHARED_LIB )

option( USE_DYNAMIC_RUNTIME "Use dynamic runtime" OFF )

if ( USE_DYNAMIC_RUNTIME )
  set( ADDITIONAL_LINK_FLAGS "" )
else ()
  if ( MINGW )
    set( ADDITIONAL_LINK_FLAGS -static-libgcc -static-libstdc++ -static )
  elseif ( MSVC )
    set( ADDITIONAL_LINK_FLAGS "" )
    foreach ( flag CMAKE_CXX_FLAGS
                   CMAKE_CXX_FLAGS_DEBUG
                   CMAKE_CXX_FLAGS_RELEASE
                   CMAKE_CXX_FLAGS_MINSIZEREL
                   CMAKE_CXX_FLAGS_RELWITHDEBINFO )
      if ( ${flag} MATCHES "/MD" )
        string( REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}" )
      endif ()
      if ( ${flag} MATCHES "/MDd" )
        string( REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}" )
      endif ()
    endforeach ()
  endif ()
endif ( USE_DYNAMIC_RUNTIME )

add_subdirectory( include )
add_subdirectory( src )

add_library( ${PROJECT_NAME} ${LIB_TYPE}
  ${ENGINE_SOURCES}
  ${ENGINE_HEADERS}
  ${IMGUI_SOURCES}
  ${LODEPNG_SOURCES}
  ${GLAD}
)

if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
  set( _CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wall -pedantic -Wextra" CACHE STRING "" FORCE )
elseif ( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
  set( _CMAKE_EXE_LINKER_FLAGS "-static -Wall -pedantic -Wextra" CACHE STRING "" FORCE )
endif()

target_include_directories( ${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glregistry_SOURCE_DIR}/api
    ${eglregistry_SOURCE_DIR}/api
    ${glm_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${glfw_SOURCE_DIR}
    ${lodepng_SOURCE_DIR}
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_link_libraries( ${PROJECT_NAME}
  PUBLIC
    ${ADDITIONAL_LINK_FLAGS}
  PRIVATE
    glfw
    jsoncpp_lib_static
    ${GL_LIB}
)

target_compile_definitions( ${PROJECT_NAME}
  PRIVATE
    ${INTERNAL_DEFINITIONS}
  PUBLIC
    ${EXTERNAL_DEFINITIONS}
)
