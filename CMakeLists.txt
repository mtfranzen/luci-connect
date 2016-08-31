CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# CMAKE
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Project
project(luci-connect)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Requirements
find_package(Threads REQUIRED)
find_package(Asio REQUIRED)
find_package(GTest REQUIRED)

# Headers
include_directories (${PROJECT_SOURCE_DIR}/include)
include_directories (${THREADS_INCLUDE_DIR})
include_directories (${ASIO_INCLUDE_DIR})

# Linker
link_directories (${PROJECT_SOURCE_DIR}/lib)
link_libraries (${THREADS_LIBRARY})

# Compiler
add_definitions(-std=c++14)
add_definitions(-DASIO_STANDALONE)
add_definitions(-DELPP_THREAD_SAFE)
add_definitions(-DELPP_DISABLE_DEFAULT_CRASH_HANDLING)
add_definitions(-Wall)

# Debug Flags
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DELPP_DISABLE_LOGS")

# Library files
add_library(luciconnect
  "${CMAKE_SOURCE_DIR}/src/node.cc"
  "${CMAKE_SOURCE_DIR}/src/core/connection.cc"
  "${CMAKE_SOURCE_DIR}/src/core/message.cc"
  "${CMAKE_SOURCE_DIR}/src/md5/md5.cc")

# Testing
if (GTEST_FOUND)
  enable_testing()
  add_executable(
    tests
    "${CMAKE_SOURCE_DIR}/test/src/main.cc"
    "${CMAKE_SOURCE_DIR}/test/src/luciconnect/core/message_tests.cc"
    "${CMAKE_SOURCE_DIR}/test/src/luciconnect/core/message_attachment_tests.cc"
    "${CMAKE_SOURCE_DIR}/test/src/luciconnect/core/connection_establish_tests.cc"
    "${CMAKE_SOURCE_DIR}/test/src/luciconnect/core/connection_send_tests.cc"
    "${CMAKE_SOURCE_DIR}/test/src/luciconnect/node_test.cc"
  )
  target_link_libraries(tests "${GTEST_BOTH_LIBRARIES}" "pthread")
  target_link_libraries(tests luciconnect)

  include_directories (${PROJECT_SOURCE_DIR}/test/include)
  include_directories (${GTEST_INCLUDE_DIRS})
endif (GTEST_FOUND)
