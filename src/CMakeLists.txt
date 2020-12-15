find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(snake display.cpp)

target_link_libraries(snake ${SDL2_LIBRARIES})