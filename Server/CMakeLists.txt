cmake_minimum_required(VERSION 3.15)
project(Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_COMPILER "E:/Regular/VS Code/LLVM/bin/clang++.exe")

set(BOOST_ROOT "D:/Libs/boost_1_87_0")

add_executable(Server main.cpp)

target_include_directories(Server PRIVATE ${BOOST_ROOT})

target_include_directories(Server PRIVATE include)

target_compile_options(Server PRIVATE -Wall -Wextra -Werror)