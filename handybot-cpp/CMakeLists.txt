Project(handybot)
cmake_minimum_required(VERSION 3.16)

set(CXX_STANDARD 11)


find_package(Threads REQUIRED)

include(FindOpenSSL)

add_definitions(-D_WEBSOCKETPP_CPP11_STL_)
include_directories(websocketpp/)

add_executable(handybot main.cpp)
target_link_libraries(handybot OpenSSL::SSL Threads::Threads ${CMAKE_DL_LIBS} )