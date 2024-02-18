cmake_minimum_required(VERSION 3.25.1)

project(Jarvis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors")
set(CMAKE_CXX_STANDARD 20)
target_compile_features(Jarvis PRIVATE cxx_std_20)

set(SOURCES
	boost_mysql.cpp
    src/Logger.cpp
    src/ResponseFactory.cpp
    src/Router.cpp
    src/Server.cpp
    src/Jarvis.cpp
)

find_package(Boost REQUIRED COMPONENTS headers)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.")
endif()

add_executable(Jarvis ${SOURCES})

target_compile_definitions(Jarvis PRIVATE BOOST_MYSQL_SEPARATE_COMPILATION)
target_include_directories(Jarvis PRIVATE inc ${Boost_INCLUDE_DIRS})
target_link_libraries(Jarvis PRIVATE Boost::headers Threads::Threads OpenSSL::Crypto OpenSSL::SSL)