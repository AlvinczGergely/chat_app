cmake_minimum_required(VERSION 3.15)

project(Chat_app)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(server)


