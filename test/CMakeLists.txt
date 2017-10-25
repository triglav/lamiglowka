add_executable(core_test slot_test.cc)
target_include_directories(core_test PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  #$<BUILD_INTERFACE:${core_SOURCE_DIR}>
  #${core_SOURCE_DIR}
  )

target_link_libraries(core_test
  core
  gtest
  gtest_main
  )

add_test(core_test core_test)
if (BUILD_COVERAGE)
  set_target_properties(core_test
    PROPERTIES
      COMPILE_FLAGS --coverage
      LINK_FLAGS --coverage
      )

  setup_target_for_coverage(
    NAME core_test_coverage
    EXECUTABLE core_test
    DEPENDENCIES core_test
    )
endif()
