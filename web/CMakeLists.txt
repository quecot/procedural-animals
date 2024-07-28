cmake_minimum_required(VERSION 3.10)
project(ProjectName LANGUAGES C VERSION 0.1.0)

message(STATUS "CMake ${CMAKE_VERSION}")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_CLANG_TIDY "clang-tidy")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(RAYLIB_INCLUDE_DIR "./raylib-5.0_macos/include")
set(RAYLIB_LIB_DIR "./raylib-5.0_macos/lib")

add_executable(main examples/procedural_snake.c)

target_include_directories(main PRIVATE ${RAYLIB_INCLUDE_DIR})
target_link_directories(main PRIVATE ${RAYLIB_LIB_DIR})
target_link_libraries(main PRIVATE raylib)
set_target_properties(main PROPERTIES
    INSTALL_RPATH "${RAYLIB_LIB_DIR}"
    BUILD_RPATH "${RAYLIB_LIB_DIR}"
)

# Add compile options
target_compile_options(main PRIVATE
    -Wall
    -Wextra
    -Wpedantic
   )
