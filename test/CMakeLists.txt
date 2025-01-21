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
    set(options)
    set(oneValueArgs)
    set(multiValueArgs LINK_LIBRARIES INCLUDE_DIRECTORIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${target_name} ${test_file})
    target_link_libraries(${target_name} PRIVATE
      gtest gmock gtest_main gcov ${ARG_LINK_LIBRARIES}
    )
    target_include_directories(${target_name} PRIVATE
      ${ARG_INCLUDE_DIRECTORIES}
    )

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

  add_test_target(task test_task.cpp
    LINK_LIBRARIES ${CMAKE_PROJECT_NAME}
  )
  add_test_target(camera test_camera.cpp
    LINK_LIBRARIES ${CMAKE_PROJECT_NAME}
  )
endif()
