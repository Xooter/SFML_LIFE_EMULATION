cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gme VERSION 1.0)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(gme main.cpp ${SRC_FILES})
target_include_directories(gme PUBLIC src/${CMAKE_CURRENT_SOURCE_DIR} )

include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system ${OPENGL_LIBRARIES})
