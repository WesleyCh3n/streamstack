if (build_tests)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  )
  FetchContent_MakeAvailable(googletest)

  # include(CTest)
  include(GoogleTest)
  enable_testing()

  add_executable(task test_task.cpp)
  target_link_libraries(task PRIVATE gtest gmock gtest_main ${3rd_libs})
  target_include_directories(task PRIVATE ${CMAKE_SOURCE_DIR}/src/ ${3rd_incs})
  # using gtest
  gtest_discover_tests(task)
  # or using ctest manually
  # add_test(NAME task_test COMMAND task)

endif()
