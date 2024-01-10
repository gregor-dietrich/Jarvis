cmake_minimum_required(VERSION 3.28)

project(
  Jarvis
  VERSION 0.1
  LANGUAGES CXX)

add_library(Logger src/Logger.cpp inc/Logger.h)
add_library(ResponseFactory src/ResponseFactory.cpp inc/ResponseFactory.h)
add_library(Router src/Router.cpp inc/Router.h)
add_library(Server src/Server.cpp inc/Server.h)
add_library(Types inc/Types.h)
add_library(Util inc/Util.h)

add_executable(Jarvis main.cpp)

target_link_libraries(Jarvis PRIVATE Logger)
target_link_libraries(Jarvis PRIVATE ResponseFactory)
target_link_libraries(Jarvis PRIVATE Router)
target_link_libraries(Jarvis PRIVATE Server)
target_link_libraries(Jarvis PRIVATE Types)
target_link_libraries(Jarvis PRIVATE Util)