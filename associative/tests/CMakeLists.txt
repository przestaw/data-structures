find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(associativeTests test_main.cpp TreeMapTests.cpp HashMapTests.cpp)
target_link_libraries(associativeTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/associativeTests)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(checkAssociative COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      --build-config "$<CONFIGURATION>"
      DEPENDS associativeTests)
else()
    add_custom_target(checkAssociative COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      DEPENDS associativeTests)
endif()
