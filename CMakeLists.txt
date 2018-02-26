cmake_minimum_required(VERSION 3.0)
project(SudokuSolver)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14")

add_subdirectory(ext/ai-cpp/src)
include_directories(ext/ai-cpp/include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

set(SRC
  src/backtracking.cc
  src/dump.cc
  src/forward-checking.cc
  src/hill-climbing.cc
  src/game.cc
  src/local-search.cc
  src/main.cc
  src/simulated-annealing.cc
)


add_executable(sudoku-solver ${SRC})
add_dependencies(sudoku-solver ai-cpp)
target_link_libraries(sudoku-solver ai-cpp ${SFML_LIBRARIES})
