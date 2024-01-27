cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(tetris)
add_executable(tetris 
  Main.cpp
  TetrisEngine.cpp
  Tetro.cpp
  )

find_package(Curses REQUIRED)

include_directories(. RasterLib ${CURSES_INCLUDE_DIRS})
target_link_libraries(tetris ${CURSES_LIBRARY})
