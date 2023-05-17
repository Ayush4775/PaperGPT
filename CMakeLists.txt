cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the curl library
find_package(CURL REQUIRED)

# Add the nlohmann_json library (assuming it is installed in a standard location)
find_package(nlohmann_json REQUIRED)

find_package(fmt REQUIRED)

include_directories(header)

# Add your source files
add_library(my_project_lib STATIC embedding.cpp prompts.cpp)
add_library(MyCppClassWrapper STATIC wrapper.mm)

# Link with the curl and nlohmann_json libraries
target_link_libraries(my_project_lib PRIVATE CURL::libcurl nlohmann_json::nlohmann_json PRIVATE fmt::fmt)
target_link_libraries(MyCppClassWrapper my_project_lib nlohmann_json::nlohmann_json CURL::libcurl)
