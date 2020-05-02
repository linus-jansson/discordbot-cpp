project(discordbot)
cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD 11)


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

#add_definitions(-DASIO_STANDALONE)
#include_directories(asio/asio/include/)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-D_WEBSOCKETPP_CPP11_STL_)
include_directories(websocketpp/)

set(THREADS_PREFER_PTHREAD_FLAG 1)
find_package(Threads REQUIRED)

add_executable(discordbot server.cpp)

target_link_libraries(discordbot
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    Threads::Threads ${CMAKE_DL_LIBS}  
)
      