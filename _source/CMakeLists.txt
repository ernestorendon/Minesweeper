cmake_minimum_required(VERSION 3.24)
project(Project_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project_3 main.cpp Game.h Game.cpp Tile.cpp Tile.h Button.h Button.cpp Overlays.h
        Overlays.cpp Digit.h Digit.cpp Random.h Random.cpp )
include_directories(boards)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Project_3 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)