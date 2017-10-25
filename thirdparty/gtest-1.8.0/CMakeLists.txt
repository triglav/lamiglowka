add_library(gtest src/gtest-all.cc include/gtest/gtest.h)
target_include_directories(gtest PUBLIC include)

add_library(gtest_main src/gtest_main.cc)
target_link_libraries(gtest_main gtest)

