cmake_minimum_required(VERSION 2.8)
project( plot )

#SET(CMAKE_BUILD_TYPE "None")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#find_package(GLM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

include_directories(src/)
aux_source_directory(src/ SOURCE_FILES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})
