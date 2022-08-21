cmake_minimum_required(VERSION 3.16)
project(fourier-draw CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
    add_compile_options($<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
    add_link_options($<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master)
FetchContent_MakeAvailable(SFML)

add_executable(fourier-draw src/main.cpp src/epicycle.cpp src/epicycles.cpp src/dft.cpp src/line.cpp)
target_link_libraries(fourier-draw PRIVATE SFML::Graphics)
target_compile_definitions(fourier-draw PRIVATE FONT_PATH="${CMAKE_SOURCE_DIR}/data")
target_compile_features(fourier-draw PRIVATE cxx_std_20)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.cpp *.h`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(run COMMAND fourier-draw)
