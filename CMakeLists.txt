cmake_minimum_required(VERSION 3.0)
project(SudokuSolver)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14")

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/ext)


ExternalProject_Add(git-ai-cpp
    GIT_REPOSITORY https://github.com/obs145628/ai-cpp.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)


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
add_dependencies(sudoku-solver git-ai-cpp)
target_link_libraries(sudoku-solver ai-cpp)
