cmake_minimum_required(VERSION 3.14)
project(Chat_app)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest fetch content for 
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(
  generator_tests
  generator_tests.cpp
  ../src/pugixml/src/pugixml.cpp
  ../src/generator.cpp
)
target_link_libraries(
  generator_tests
  GTest::gtest_main
)

target_include_directories(generator_tests PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

include(GoogleTest)
gtest_discover_tests(generator_tests)