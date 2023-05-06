cmake_minimum_required(VERSION 3.10)
project(my_project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find the curl library
find_package(CURL REQUIRED)

# Add the nlohmann_json library (assuming it is installed in a standard location)
find_package(nlohmann_json REQUIRED)

include_directories(header)

# Add your source files
add_executable(my_project main.cpp prompts.cpp embedding.cpp)

# Link with the curl and nlohmann_json libraries
target_link_libraries(my_project PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
