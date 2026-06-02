cmake_minimum_required(VERSION 3.28)

project(mephi_lab4)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/CardinalTests.cpp
    tests/RecurentGeneratorTests.cpp
    tests/LazySequenceTests.cpp
)

target_link_libraries(tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(mephi_lab4 PRIVATE ${CURSES_LIBRARIES})

add_executable(main
    main.cpp
)