if (NOT ASIO_FOUND)
	# Try standalone Asio first
	find_path (ASIO_INCLUDE_DIR NAMES asio HINTS "/usr/include" "/usr/local/include" "/opt/local/include")

	if (NOT ASIO_INCLUDE_DIR)
	  # Try Boost.Asio
	  find_package(Boost COMPONENTS system REQUIRED)
	  set (ASIO_STANDALONE FALSE)
	else()
	  set (ASIO_STANDALONE TRUE)
	endif()

	if (ASIO_STANDALONE)
		add_library(asio INTERFACE IMPORTED)
		set_target_properties(asio PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${ASIO_INCLUDE_DIR}
		)
	else()
		add_library(asio INTERFACE IMPORTED)
		set_target_properties(asio PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
			INTERFACE_LINK_LIBRARIES ${Boost_SYSTEM_LIBRARY}
		)
	endif()
endif()
