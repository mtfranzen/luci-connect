if (WIN32)
  #TODO: Find asio on Windows
else (WIN32)
  find_path (ASIO_INCLUDE_DIR NAMES asio HINTS "/usr/include" "/usr/local/include" "/opt/local/include")
  set (ASIO_LIBRARY ${ASIO_INCLUDE_DIR})
endif (WIN32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Asio DEFAULT_MSG ASIO_LIBRARY ASIO_INCLUDE_DIR)
mark_as_advanced(VULKAN_INCLUDE_DIR VULKAN_LIBRARY)
