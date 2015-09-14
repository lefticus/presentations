cmake_minimum_required(VERSION 2.8)

project(crazy_easy_examples)


# MINGW does not yet support C++11's concurrency features
if(MINGW)
  option(MULTITHREAD_SUPPORT_ENABLED "Multithreaded Support Enabled" FALSE)
else()
  option(MULTITHREAD_SUPPORT_ENABLED "Multithreaded Support Enabled" TRUE)
endif()


if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable Coverage Reporting in GCC" FALSE)

  if(ENABLE_COVERAGE)
    add_definitions(--coverage -O0)
    set(LINKER_FLAGS "${LINKER_FLAGS} --coverage")
  endif()
endif()


#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/readme.md")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/description.txt")

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 1)

set(CPACK_PACKAGE_EXECUTABLES "game_engine;Game Engine Interperter")
set(CPACK_PACKAGE_VENDOR "ChaiScript.com")
set(CPACK_PACKAGE_CONTACT "contact@chaiscript.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A game engine")

set(CPACK_DEBIAN_PACKAGE_SECTION "game")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_GROUP "Gaming")

set(CHAI_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

#configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)

include(CTest)
include(CPack)

enable_testing()

if(CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

  if(GCC_VERSION VERSION_LESS 4.8)
    set(CPP11_FLAG "-std=c++0x")
  else()
    set(CPP11_FLAG "-std=c++11")
  endif()
else()
  set(CPP11_FLAG "-std=c++11")
endif()

if(MSVC)
  add_definitions(/W4 /w44640)
  add_definitions(/bigobj)
  # Note on MSVC compiler flags.
  # The code base selective disables warnings as necessary when the compiler is complaining too much
  # about something that is perfectly valid, or there is simply no technical way around it
  # This particular warning, C4503 is in regards to the decorated names that MSVC generates internally.
  # The error did not come up until the move to C++11, but the compiler doesn't give enough information
  # to determine where the error is coming from, and the internet provides no real information for
  # how to workaround or fix the error. So I'm disabling it globally.
  add_definitions(/wd4503)
else()
  add_definitions(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic ${CPP11_FLAG})

  if(APPLE)
    add_definitions(-Wno-sign-compare)
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  option(USE_LIBCXX "Use clang's libcxx" FALSE)

  if(USE_LIBCXX)
    add_definitions(-stdlib=libc++)
    set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP11_FLAG} -stdlib=libc++")
  else()
    set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP11_FLAG}")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP11_FLAG}")
endif()

# limitations in MinGW require us to make an optimized build
# for the sake of object sizes or something
if(MINGW OR CYGWIN)
  add_definitions(-O3)
endif()

if(UNIX AND NOT APPLE)
  set(LINKER_FLAGS "${LINKER_FLAGS} -fuse-ld=gold")
endif()


include_directories(include)

if(NOT MULTITHREAD_SUPPORT_ENABLED)
  add_definitions(-DCHAISCRIPT_NO_THREADS -DCHAISCRIPT_NO_THREADS_WARNING)
endif()

if(CMAKE_HOST_UNIX)
  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
    if (NOT MINGW AND NOT CYGWIN)
      list(APPEND LIBS "dl")
    endif()
  endif()

  if(MULTITHREAD_SUPPORT_ENABLED)
    if(CMAKE_COMPILER_IS_GNUCC)
      execute_process(COMMAND ${CMAKE_C_COMPILER} --version OUTPUT_VARIABLE GCC_FULL_VERSION)
      if(GCC_FULL_VERSION MATCHES "4.8.1.*ubuntu")
        set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--no-as-needed -pthread")
      else()
        set(LINKER_FLAGS "${LINKER_FLAGS} -pthread")
      endif()
    else()
      set(LINKER_FLAGS "${LINKER_FLAGS} -pthread")
    endif()

    add_definitions(-pthread)
  endif()
endif()


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

add_library(stdlib OBJECT chaiscript_stdlib.cpp)

add_executable(basics basics.cpp $<TARGET_OBJECTS:stdlib>)
target_link_libraries(basics ${LIBS})

add_executable(sharing_data sharing_data.cpp $<TARGET_OBJECTS:stdlib>)
target_link_libraries(sharing_data ${LIBS})

add_executable(sharing_functions sharing_functions.cpp $<TARGET_OBJECTS:stdlib>)
target_link_libraries(sharing_functions ${LIBS})

add_executable(sharing_exceptions sharing_exceptions.cpp $<TARGET_OBJECTS:stdlib>)
target_link_libraries(sharing_exceptions ${LIBS})

add_executable(sharing_futures sharing_futures.cpp $<TARGET_OBJECTS:stdlib>)
target_link_libraries(sharing_futures ${LIBS})



