cmake_minimum_required(VERSION 3.10)

project(snake)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

set_target_properties(snake PROPERTIES
CXX_STANDARD 11
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)