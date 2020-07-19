Project(handybot CXX)
cmake_minimum_required(VERSION 3.16)

set(CXX_STANDARD 11)


find_package(Threads REQUIRED)

# find_package(Boost 1.71.0 COMPONENTS system)

include(FindOpenSSL REQUIRED)
# include_directories(${OPENSSL_INCLUDE_DIR})

add_subdirectory(cpr)

add_definitions(-D_WEBSOCKETPP_CPP11_STL_)
include_directories(websocketpp/)

add_library(handylib 
        client.hpp)
set_target_properties(handylib PROPERTIES LINKER_LANGUAGE CXX)

# ${Boost_LIBRARIES}
add_executable(handybot main.cpp)
target_link_libraries(handybot handylib cpr OpenSSL::SSL OpenSSL::Crypto Threads::Threads ${CMAKE_DL_LIBS})