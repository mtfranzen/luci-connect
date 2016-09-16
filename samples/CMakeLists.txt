CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# CMAKE
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Project
project(luci-connect-samples)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Requirements
find_package(Threads REQUIRED)

# Headers
include_directories (${THREADS_INCLUDE_DIR})

# Linker
link_libraries (luciconnect)

if (WIN32)
  link_libraries (${THREAD_LIBRARY})
else (WIN32)
  link_libraries (pthread)
endif (WIN32)

# Compiler
add_definitions(-std=c++14)
add_definitions(-Wall)
add_definitions(-DELPP_THREAD_SAFE)
add_definitions(-DELPP_DISABLE_DEFAULT_CRASH_HANDLING)

# Library
add_executable(simple-client "${CMAKE_SOURCE_DIR}/src/simple-client.cc")
add_executable(simple-service "${CMAKE_SOURCE_DIR}/src/simple-service.cc")
