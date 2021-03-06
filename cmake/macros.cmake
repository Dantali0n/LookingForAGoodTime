# enable cxx11 using any version of CMake #
macro(use_cxx11)
	if(CMAKE_VERSION VERSION_LESS "3.1")
		message("CMake < 3.1 setting cxx flags for c++11 manually")
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
		elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
			
		endif()
	else()
		set(CMAKE_CXX_STANDARD 11)
	endif()
endmacro(use_cxx11)

#all warnings and errors
macro(use_cxx_warning_pedantic)
	if(MSVC)
		# Force to always compile with W4
		message("Found MSVC enabling W4")
		if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
			string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		else()
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
		endif()
	elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		# Update if necessary
		message("Found GCC enabling pedantic")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
	endif()
endmacro(use_cxx_warning_pedantic)