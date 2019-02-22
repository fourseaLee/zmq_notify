cmake_minimum_required(VERSION 3.5)
project(zmq_notify)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
option(COMPILE_LIB "compile library" off)
message(STATUS "compile library status(COMPILE_LIB):" ${COMPILE_LIB})



if(COMPILE_LIB)
    include_directories( zmq_notify)
    add_subdirectory(zmq_notify)
else()
    include_directories(zmq_notify)
    #set(ZMQ_NOTIFY_LIB build/zmq_notify)
    #target_include_directories(zmq_notify
    #        PUBLIC
    #        .
            # To access the config.
    #        ${ZMQ_NOTIFY_LIB}
    #        )

  #  FIND_LIBRARY(ZMQ_NOTIFY_LIB zmq_notify build/zmq_notify)

 #   link_libraries(${ZMQ_NOTIFY_LIB})
    #add_executable(zmq_test main.cpp)

    #list(APPEND CMAKE_LIBRARY_PATH "/home/dev/workspace/zmq_notify/build/zmq_notify") 
#    LINK_DIRECTORIES(/home/dev/worksapce/zmq_notify/build/zmq_notify)
    add_executable(zmq_test main.cpp)
   # LINK_DIRECTORIES(/home/dev/workspace/zmq_notify/build/zmq_notify)
    target_link_libraries(zmq_test zmq_notify zmq boost_system boost_chrono boost_thread)
endif(COMPILE_LIB)
