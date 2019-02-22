cmake_minimum_required(VERSION 3.0)
project(notify)

file(GLOB ZMQ_NOTIFY_SRC 
        *.cpp  *.h
        compat/*.cpp compat/*.h
        support/*.cpp support/*.h
        support/allocators/*.h
    )

add_library(zmq_notify ${ZMQ_NOTIFY_SRC})
#add_library(zmq_notify ${ZMQ_NOTIFY_SRC})
target_link_libraries(zmq_notify zmq boost_system)


