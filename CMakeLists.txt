cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)
project(lamiglowka)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glog 0.3.5 REQUIRED)

add_executable(lamiglowka src/main.cc)
target_link_libraries(lamiglowka glog::glog)

