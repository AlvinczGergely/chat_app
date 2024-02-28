cmake_minimum_required(VERSION 3.15)
project(Chat_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
set(SQLITECPP_RUN_CPPLINT OFF CACHE INTERNAL "") 
FetchContent_Declare(
  SQLiteCpp
  GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
  GIT_TAG        3.3.1
)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
  Pistache
  GIT_REPOSITORY https://github.com/pistacheio/pistache.git
)
FetchContent_MakeAvailable(Pistache)

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(SOURCES "main.cpp" "generator.cpp" "user.cpp" "services.cpp" "pugixml/src/pugixml.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME}
  PUBLIC
    SQLiteCpp
    sqlite3
    pistache
  PRIVATE 
    nlohmann_json::nlohmann_json
  )

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${SQLiteCpp_SOURCE_DIR}/include
  ${Pistache_SOURCE_DIR}/include
  )

