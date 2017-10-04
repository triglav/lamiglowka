cmake_minimum_required(VERSION 3.2.2 FATAL_ERROR)
project(lamiglowka)

find_package(glog 0.3.5 REQUIRED)

add_executable(lamiglowka src/main.cc)
target_link_libraries(lamiglowka glog::glog)

