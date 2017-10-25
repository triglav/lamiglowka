cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)
project(lamiglowka)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(glog 0.3.5 REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(thirdparty)

