find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(linearTests test_main.cpp VectorTests.cpp LinkedListTests.cpp )
target_link_libraries(linearTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun linearTests)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(checkLinear COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      --build-config "$<CONFIGURATION>"
      DEPENDS linearTests)
else()
    add_custom_target(checkLinear COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      DEPENDS linearTests)
endif()
