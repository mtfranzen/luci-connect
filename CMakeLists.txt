CMAKE_MINIMUM_REQUIRED (VERSION 3.1)

# CMAKE
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Project
project(luci-connect)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 1)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Requirements
find_package(Threads REQUIRED)
find_package(Asio REQUIRED)
find_package(GTest)

# Headers
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

if(THREADS_HAVE_PTHREAD_ARG)
  compile_options(PUBLIC"-pthread")
endif()

if(CMAKE_THREAD_LIBS_INIT)
  link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

# Linker
link_directories (${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(THREADS_HAVE_PTHREAD_ARG)
  compile_options(PUBLIC"-pthread")
endif()

if(CMAKE_THREAD_LIBS_INIT)
  link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

# Compiler
set (CMAKE_CXX_STANDARD 11)
if (ASIO_STANDALONE)
  add_definitions(-DASIO_STANDALONE=1)
endif()
add_definitions(-DELPP_THREAD_SAFE)
add_definitions(-DELPP_DISABLE_DEFAULT_CRASH_HANDLING)
add_definitions(-Wall)

if (WIN32)
   add_definitions(-DELPP_EXPORT)
endif (WIN32)

# Release Flags
if (WIN32)
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DELPP_DISABLE_LOGS")
else ()
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DELPP_DISABLE_LOGS")
endif()

# Library files
add_library(luciconnect SHARED
  "${CMAKE_CURRENT_SOURCE_DIR}/src/node.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/connection.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/message.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/quaview-service.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/md5/md5.cc")
target_link_libraries(luciconnect PUBLIC asio)

# Testing
if (GTEST_FOUND)
  enable_testing()
  add_executable(
    tests
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/main.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/luciconnect/message_tests.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/luciconnect/message_attachment_tests.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/luciconnect/connection_establish_tests.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/luciconnect/connection_send_tests.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/luciconnect/node_test.cc"
  )

  target_link_libraries(tests ${GTEST_BOTH_LIBRARIES})
  target_link_libraries(tests luciconnect)

  include_directories (${PROJECT_SOURCE_DIR}/test/include)
  include_directories (${GTEST_INCLUDE_DIRS})
else (GTEST_FOUND)
  message(WARNING "Not compiling tests")
endif (GTEST_FOUND)

# Copy files to include-directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/easylogging/src/easylogging++.h DESTINATION include/luciconnect/easylogging)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/json/src/json.hpp DESTINATION include/luciconnect/json)


# Installation
set (CMAKE_INSTALL_PREFIX "/usr/")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/luciconnect DESTINATION include COMPONENT headers)
install(TARGETS luciconnect DESTINATION lib COMPONENT libraries)

# Packaging
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_CONTACT "franzen@arch.ethz.ch")
set (CPACK_PACKAGE_MAINTAINER "Michael Franzen <franzen@arch.ethz.ch>")
set (CPACK_PACKAGE_NAME "libluciconnect-dev")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library and C++-Header files to develop services and clients for Luci" )
set (CPACK_COMPONENTS_ALL libraries headers)
set (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Library")
set (CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Shared Library for Luciconnect")
set (CPACK_COMPONENT_LIBRARIES_REQUIRED Asio)
set (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")
set (CPACK_COMPONENT_HEADERS_DESCRIPTION "C++ Headers for Luciconnect")
set (CPACK_COMPONENT_HEADERS_DEPENDS libraries)
set (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set (CPACK_COMPONENT_HEADERS_GROUP "Development")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set (CPACK_PACKAGE_VERSION "${VERSION}")

if (WIN32)
  set (CPACK_GENERATOR "NSIS")
else (WIN32)
  set (CPACK_GENERATOR "DEB")
endif (WIN32)

include (CPack)
