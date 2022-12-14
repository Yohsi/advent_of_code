cmake_minimum_required(VERSION 3.20)
project(2021)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/matplotlib)
link_libraries(matplotlib_cpp)

include_directories(include)

if (${CMAKE_BUILD_TYPE} EQUAL Debug)
    add_compile_definitions(_GLIBCXX_DEBUG=1)
endif ()

add_executable(day01 1/main.cc)
target_compile_definitions(day01 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/1")
add_executable(day02 2/main.cc)
target_compile_definitions(day02 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/2")
add_executable(day03 3/main.cc)
target_compile_definitions(day03 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/3")
add_executable(day04 4/main.cc)
target_compile_definitions(day04 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/4")
add_executable(day05 5/main.cc)
target_compile_definitions(day05 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/5")
add_executable(day06 6/main.cc)
target_compile_definitions(day06 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/6")
add_executable(day07 7/main.cc)
target_compile_definitions(day07 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/7")
add_executable(day08 8/main.cc)
target_compile_definitions(day08 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/8")
add_executable(day09 9/main.cc)
target_compile_definitions(day09 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/9")
add_executable(day10 10/main.cc)
target_compile_definitions(day10 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/10")
add_executable(day11 11/main.cc)
target_compile_definitions(day11 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/11")
add_executable(day12 12/main.cc)
target_compile_definitions(day12 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/12")
add_executable(day13 13/main.cc)
target_compile_definitions(day13 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/13")
add_executable(day14 14/main.cc)
target_compile_definitions(day14 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/14")
add_executable(day15 15/main.cc)
target_compile_definitions(day15 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/15")
add_executable(day16 16/main.cc)
target_compile_definitions(day16 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/16")
add_executable(day17 17/main.cc)
target_compile_definitions(day17 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/17")
add_executable(day18 18/main.cc)
target_compile_definitions(day18 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/18")
add_executable(day19 19/main.cc)
target_compile_definitions(day19 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/19")
add_executable(day20 20/main.cc)
target_compile_definitions(day20 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/20")
add_executable(day21 21/main.cc)
target_compile_definitions(day21 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/21")
add_executable(day22 22/main.cc)
target_compile_definitions(day22 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/22")
add_executable(day23 23/main.cc)
target_compile_definitions(day23 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/23")
add_executable(day24 24/main.cc)
target_compile_definitions(day24 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/24")
add_executable(day25 25/main.cc)
target_compile_definitions(day25 PRIVATE INPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/25")


