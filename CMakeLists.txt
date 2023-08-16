cmake_minimum_required (VERSION 3.5)

project (hello_world)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
set (OpenGL_GL_PREFERENCE "GLVND") #Alternatevly we can use LEGACY(but we realy shouldn't)

file (GLOB_RECURSE source_files "${source_dir}/*.cpp")

add_executable (hello_world ${source_files})


#linking GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(GLEW REQUIRED)
target_link_libraries(hello_world GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(hello_world OpenGL::GL)

add_subdirectory(libs/glfw)
target_link_libraries(hello_world glfw)



