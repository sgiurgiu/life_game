cmake_minimum_required(VERSION 2.6)
project(life_game)

add_executable(life_game main.cpp)

install(TARGETS life_game RUNTIME DESTINATION bin)
