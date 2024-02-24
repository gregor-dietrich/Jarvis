cmake_minimum_required(VERSION 3.22.1)

project(Jarvis VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors")
set(CMAKE_CXX_STANDARD 20)
set(BOOST_MIN_VERSION 1.82.0)
set(OPENSSL_MIN_VERSION 3.0.11)

set(SOURCES
    src/Logger.cpp
    src/ResponseFactory.cpp
    src/Router.cpp
    src/Server.cpp
    src/Jarvis.cpp
)

# Boost Package
find_package(Boost ${BOOST_MIN_VERSION})
foreach(MAJOR RANGE 82 99)
	if(Boost_FOUND)
		break()
	endif()
	foreach(MINOR RANGE 5)
		if(Boost_FOUND)
			break()
		endif()
		set(BOOST_ROOT "/usr/local/boost_1_${MAJOR}_${MINOR}")
		find_package(Boost ${BOOST_MIN_VERSION} QUIET)
	endforeach()
endforeach()
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)

# Other Packages
find_package(Threads REQUIRED)
find_package(OpenSSL ${OPENSSL_MIN_VERSION} REQUIRED)

# Target
add_executable(Jarvis ${SOURCES})
target_compile_definitions(Jarvis PRIVATE BOOST_BIND_GLOBAL_PLACEHOLDERS BOOST_MYSQL_SEPARATE_COMPILATION)
target_compile_features(Jarvis PRIVATE cxx_std_${CMAKE_CXX_STANDARD})
target_include_directories(Jarvis PRIVATE inc ${Boost_INCLUDE_DIRS})
target_link_libraries(Jarvis PRIVATE Threads::Threads OpenSSL::Crypto OpenSSL::SSL)