cmake_minimum_required( VERSION 3.12.1 FATAL_ERROR )

FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY "https://github.com/open-source-parsers/jsoncpp.git"
  GIT_TAG        "0.10.7"
)

FetchContent_GetProperties( jsoncpp )

if ( NOT jsoncpp_POPULATED )
  FetchContent_Populate( jsoncpp )
  message( STATUS "jsoncpp source dir: ${jsoncpp_SOURCE_DIR}" )
  message( STATUS "jsoncpp binary dir: ${jsoncpp_BINARY_DIR}" )

  set( JSONCPP_WITH_TESTS OFF CACHE INTERNAL "" )
  set( JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "" )
  set( JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE INTERNAL "" )

  add_subdirectory( ${jsoncpp_SOURCE_DIR} ${jsoncpp_BINARY_DIR} )
endif()
