cmake_minimum_required ( VERSION 3.12.1 FATAL_ERROR )
project ( engine LANGUAGES CXX )

include ( FetchContent )



FetchContent_Declare (
  glregistry
  GIT_REPOSITORY https://github.com/KhronosGroup/OpenGL-Registry.git
)

FetchContent_GetProperties ( glregistry )
if ( NOT glregistry_POPULATED )
  FetchContent_Populate ( glregistry )
  message( STATUS "Catch source dir: ${glregistry_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${glregistry_BINARY_DIR}" )
endif()

FetchContent_Declare (
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG        "0.9.9.5"
)

FetchContent_GetProperties ( glm ) #mispelled name in original post
if ( NOT glm_POPULATED )
  FetchContent_Populate ( glm )
  message( STATUS "Catch source dir: ${glm_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${glm_BINARY_DIR}" )
endif()

FetchContent_Declare (
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG        "v1.69"
)

FetchContent_GetProperties ( imgui ) #mispelled name in original post
if ( NOT imgui_POPULATED )
  FetchContent_Populate ( imgui )
  message( STATUS "Catch source dir: ${imgui_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${imgui_BINARY_DIR}" )
  
  set ( IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  )
endif()

FetchContent_Declare (
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG        "3.2.1"
)

FetchContent_GetProperties ( glfw ) #mispelled name in original post
if ( NOT glfw_POPULATED )
  FetchContent_Populate ( glfw )
  message( STATUS "Catch source dir: ${glfw_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${glfw_BINARY_DIR}" )
  set ( GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" )
  set ( GLFW_BUILD_TESTS OFF CACHE INTERNAL "" )
  set ( GLFW_BUILD_DOCS OFF CACHE INTERNAL "" )
  add_subdirectory ( ${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} ) #can be case insensitive
endif()

FetchContent_Declare (
  lodepng
  GIT_REPOSITORY "https://github.com/lvandeve/lodepng.git"
)

FetchContent_GetProperties ( lodepng ) #mispelled name in original post
if ( NOT lodepng_POPULATED )
  FetchContent_Populate ( lodepng )
  message( STATUS "Catch source dir: ${lodepng_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${lodepng_BINARY_DIR}" )

  set ( LODEPNG_SOURCES
    ${lodepng_SOURCE_DIR}/lodepng.cpp
  )
endif()

FetchContent_Declare (
  jsoncpp
  GIT_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git"
  GIT_TAG        "0.10.7"
)

FetchContent_GetProperties ( jsoncpp ) #mispelled name in original post
if ( NOT jsoncpp_POPULATED )
  FetchContent_Populate ( jsoncpp )
  message( STATUS "Catch source dir: ${jsoncpp_SOURCE_DIR}" )
  message( STATUS "Catch binary dir: ${jsoncpp_BINARY_DIR}" )
  set ( JSONCPP_WITH_TESTS OFF CACHE INTERNAL "" )
  set ( JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "" )
  set ( JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE INTERNAL "" )
  add_subdirectory ( ${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR} ) #can be case insensitive
endif()

if ( UNIX )
  set ( GL_LIB GL )
elseif ( WIN32 )
  set ( GL_LIB OpenGL32 )
endif ()

option ( BUILD_SHARED_LIB "Build shared library" OFF )
option ( USE_DYNAMIC_RUNTIME "Use dynamic runtime" OFF )

if ( BUILD_SHARED_LIB )
  set ( INTERNAL_DEFINITIONS
    ${INTERNAL_DEFINITIONS}
    ENGINE_LIB_SHARED
    BUILD_DLL
  )
  set ( EXTERNAL_DEFINITIONS
    ${EXTERNAL_DEFINITIONS}
    ENGINE_LIB_SHARED
  )
  set ( LIB_TYPE SHARED )
else ()
  set ( LIB_TYPE STATIC )
endif ( BUILD_SHARED_LIB )

if ( USE_DYNAMIC_RUNTIME )
  set ( ADDITIONAL_LINK_FLAGS "" )
else ()
  if ( MINGW )
    set ( ADDITIONAL_LINK_FLAGS -static-libgcc -static-libstdc++ -static )
  elseif ( MSVC )
    set ( ADDITIONAL_LINK_FLAGS "" )
    foreach ( flag CMAKE_CXX_FLAGS
                   CMAKE_CXX_FLAGS_DEBUG
                   CMAKE_CXX_FLAGS_RELEASE
                   CMAKE_CXX_FLAGS_MINSIZEREL
                   CMAKE_CXX_FLAGS_RELWITHDEBINFO )
      if ( ${flag} MATCHES "/MD" )
        string ( REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}" )
      endif ()
      if ( ${flag} MATCHES "/MDd" )
        string ( REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}" )
      endif ()
    endforeach ()
  endif ()
endif ( USE_DYNAMIC_RUNTIME )

add_subdirectory ( include )
add_subdirectory ( src )

add_library ( engine ${LIB_TYPE}
  ${ENGINE_SOURCES}
  ${ENGINE_HEADERS}
  ${IMGUI_SOURCES}
  ${LODEPNG_SOURCES}
)

target_include_directories ( engine
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glregistry_SOURCE_DIR}
    ${glm_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${glfw_SOURCE_DIR}
    ${lodepng_SOURCE_DIR}
)

set_target_properties ( engine PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_link_libraries ( engine
  PUBLIC
    ${ADDITIONAL_LINK_FLAGS}
  PRIVATE
    glfw
    jsoncpp_lib_static
    ${GL_LIB}
)

target_compile_definitions ( engine
  PRIVATE
    ${INTERNAL_DEFINITIONS}
  PUBLIC
    ${EXTERNAL_DEFINITIONS}
)
