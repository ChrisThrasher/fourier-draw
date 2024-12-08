cmake_minimum_required(VERSION 3.22)
project(fourier-draw CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git)
FetchContent_MakeAvailable(SFML)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
elseif(MSVC)
    add_compile_options(/WX /W4 /permissive-)
endif()

add_executable(fourier-draw src/main.cpp src/Epicycle.cpp src/Epicycles.cpp src/Dft.cpp src/Line.cpp)
target_link_libraries(fourier-draw PRIVATE SFML::Graphics)
target_compile_definitions(fourier-draw PRIVATE FONT_PATH="${CMAKE_SOURCE_DIR}/data")
target_compile_features(fourier-draw PRIVATE cxx_std_20)

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.cpp *.hpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(run COMMAND fourier-draw)
