cmake_minimum_required(VERSION 3.28.1)

project(Jarvis VERSION 0.1 LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS system)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found.")
endif()

set(SOURCES
    src/Logger.cpp
    src/ResponseFactory.cpp
    src/Router.cpp
    src/Server.cpp
    src/Jarvis.cpp
)

add_executable(Jarvis ${SOURCES})

target_compile_features(Jarvis PRIVATE cxx_std_20)

target_include_directories(Jarvis PUBLIC inc ${Boost_INCLUDE_DIRS})