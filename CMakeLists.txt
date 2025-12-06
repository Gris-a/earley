cmake_minimum_required(VERSION 3.14)
project(Earley)
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_COVERAGE "Enable coverage" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(-g -O0 --coverage)
    add_link_options(--coverage)
endif()

add_library(earley src/earley.cpp src/earley_situation.cpp)
target_include_directories(earley PUBLIC include)

add_executable(parser app/main.cpp)
target_link_libraries(parser earley)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

add_executable(earley_tests test/earley_test.cpp)
target_link_libraries(earley_tests earley GTest::gtest_main)
target_include_directories(earley_tests PRIVATE include)

enable_testing()
add_test(NAME EarleyParserTests COMMAND earley_tests)

if(ENABLE_COVERAGE)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    if(LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND mkdir -p coverage
            COMMAND ${CMAKE_BINARY_DIR}/earley_tests
            COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --include '${CMAKE_SOURCE_DIR}/src/**' --output-file coverage/coverage.info --ignore-errors mismatch
            COMMAND genhtml -o coverage coverage/coverage.info
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS earley_tests
            COMMENT "Generating coverage report"
        )
    endif()
endif()