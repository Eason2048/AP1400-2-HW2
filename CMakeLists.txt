cmake_minimum_required(VERSION 3.13)
project(AP)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
set(GTest_ROOT /opt/homebrew/opt/googletest)
find_package(GTest REQUIRED)

include_directories(include/)
include_directories(/opt/homebrew/opt/googletest/include /opt/homebrew/opt/googletest)

add_executable(main
        src/main.cpp
        src/server.cpp
        src/client.cpp
        src/crypto.cpp
        src/unit_test.cpp
)
target_link_libraries(main
        OpenSSL::SSL
        /opt/homebrew/opt/googletest/lib/libgtest.a
        /opt/homebrew/opt/googletest/lib/libgtest_main.a
)