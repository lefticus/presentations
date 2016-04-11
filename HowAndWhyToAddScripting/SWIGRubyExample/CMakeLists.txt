cmake_minimum_required (VERSION 2.8.6)


project (EmbeddedScripting CXX)

find_program(SWIG_EXECUTABLE NAMES swig3.0 swig2.0 swig)
find_package(SWIG REQUIRED)


find_package(Ruby 1.9 REQUIRED)

set(BUILD_SHARED_LIBS OFF)

set_source_files_properties(EmbeddedScripting.i
  PROPERTIES CPLUSPLUS true
  )

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pthread ")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

include_directories(${RUBY_INCLUDE_DIR} ${RUBY_CONFIG_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )


add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/EmbeddedScriptingRUBY_wrap.cxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby"
          "-c++"
          -o "${CMAKE_CURRENT_BINARY_DIR}/EmbeddedScriptingRUBY_wrap.cxx"
          "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
          "exposed_code.hpp"
)


add_executable(EmbeddedScripting
 "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
 main.cpp
 exposed_code.hpp
 exposed_code.cpp
  "${CMAKE_CURRENT_BINARY_DIR}/EmbeddedScriptingRUBY_wrap.cxx"
 )


if (UNIX AND NOT APPLE)
  target_link_libraries(EmbeddedScripting ${RUBY_LIBRARY}  "dl" "crypt")
else()
  target_link_libraries(EmbeddedScripting ${RUBY_LIBRARY} )
endif()


