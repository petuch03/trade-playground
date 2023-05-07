cmake_minimum_required(VERSION 3.20)
project(trade)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(CURL REQUIRED)

add_executable(trade main.cpp)
target_link_libraries(trade PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(trade PRIVATE CURL::libcurl)
