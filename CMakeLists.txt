cmake_minimum_required(VERSION 3.25.1)

project(Jarvis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors")
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/Logger.cpp
    src/ResponseFactory.cpp
    src/Router.cpp
    src/Server.cpp
    src/Jarvis.cpp
)

add_executable(Jarvis ${SOURCES})

target_compile_features(Jarvis PRIVATE cxx_std_20)

add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

find_package(Boost REQUIRED COMPONENTS system)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.")
endif()

target_include_directories(Jarvis PUBLIC inc ${Boost_INCLUDE_DIRS})