if (build_tests)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  )
  FetchContent_MakeAvailable(googletest)

  include(GoogleTest)
  enable_testing()

  function(add_test_target target_name test_file)
    add_executable(${target_name} ${test_file})
    target_link_libraries(${target_name} PRIVATE gtest gmock gtest_main gcov ${3rd_libs})
    target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/src/ ${3rd_incs})
    target_compile_options(${target_name} PRIVATE
      "$<$<CXX_COMPILER_ID:GNU>:-g;-O0;-ftest-coverage;-fprofile-arcs;--coverage>"
    )
    target_link_options(${target_name} PRIVATE
      "$<$<CXX_COMPILER_ID:GNU>:--coverage>"
    )
    gtest_discover_tests(${target_name})
    # or using ctest manually
    # add_test(NAME task_test COMMAND task)
  endfunction()

  add_test_target(task test_task.cpp)
endif()
