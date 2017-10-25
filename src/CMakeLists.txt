add_executable(lamiglowka main.cc)
target_link_libraries(lamiglowka
  PRIVATE
    core
    glog::glog
    )

add_subdirectory(core)

