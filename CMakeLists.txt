project(discordbot)
cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD 11)

add_definitions(-DASIO_STANDALONE)
include_directories(asio-1.16.1)

#find_package(Boost REQUIRED COMPONENTS system)
#include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-D_WEBSOCKETPP_CPP11_STL_)

set(THREADS_PREFER_PTHREAD_FLAG 1)
find_package(Threads REQUIRED)

add_executable(discordbot main.cpp)

target_link_libraries(discordbot
#    ${Boost_LIBRARIES}
    Threads::Threads ${CMAKE_DL_LIBS}  
)
      