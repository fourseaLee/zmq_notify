cmake_minimum_required(VERSION 3.0)
project(libnotify)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

file(GLOB NOTIFY_SRC  *.cpp *.h)

add_library(notify ${NOTIFY_SRC})

target_link_libraries(notify zmq boost_system)
