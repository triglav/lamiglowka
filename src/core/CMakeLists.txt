add_library(core INTERFACE)
target_include_directories(core INTERFACE .)
target_link_libraries(core
  INTERFACE
    FastDelegate
    glog::glog
    )

