cmake_minimum_required(VERSION 3.20)
project(trade-playground)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(CURL REQUIRED)

add_executable(trade-playground main.cpp)
target_link_libraries(trade-playground PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(trade-playground PRIVATE CURL::libcurl)
