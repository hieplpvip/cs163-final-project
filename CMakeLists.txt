cmake_minimum_required(VERSION 3.10)
project(mse)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories( ./src )

file(GLOB_RECURSE cpp_files CONFIGURE_DEPENDS "src/*.cpp")
add_executable( mse ${cpp_files} )
