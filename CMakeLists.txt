project(life_game)
cmake_minimum_required(VERSION 3.2)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

# Add global definitions
add_definitions("-Wall -std=c++17 -pedantic -Wextra")

# Find SDL2
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

set(LIFE_SRC main.cpp
            )

add_executable(${PROJECT_NAME} ${LIFE_SRC})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
