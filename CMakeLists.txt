project(life_game)
cmake_minimum_required(VERSION 3.2)


# Add global definitions
add_definitions("-Wall -std=c++17 -pedantic -Wextra")

find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SFML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

set(LIFE_SRC main.cpp
            )

add_executable(${PROJECT_NAME} ${LIFE_SRC})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})
