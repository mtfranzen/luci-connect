if (NOT ASIO_FOUND)
  # Try standalone Asio first
  find_path (ASIO_INCLUDE_DIR NAMES asio HINTS "/usr/include" "/usr/local/include" "/opt/local/include")

  if (NOT ASIO_INCLUDE_DIR)
    # Try Boost.Asio
	set(Boost_USE_STATIC_LIBS ON)
	set(BOOST_COMPONENTS_NEEDED system date_time regex)
    find_package(Boost COMPONENTS system date_time regex REQUIRED)
    set (ASIO_STANDALONE FALSE)
  else()
    set (ASIO_STANDALONE TRUE)
  endif()

  if (ASIO_STANDALONE)
    add_library(asio STATIC IMPORTED)
    set_target_properties(asio PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES ${ASIO_INCLUDE_DIR}
    )
  else()
    add_library(asio STATIC IMPORTED)
    set_target_properties(asio PROPERTIES
	  INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}"
  	  INTERFACE_LINK_LIBRARY "${Boost_SYSTEM_LIBRARY}"
      IMPORTED_LOCATION "${BOOST_LIBRARYDIR}"
    )
  endif()
endif()
