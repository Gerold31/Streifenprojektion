cmake_minimum_required(VERSION 2.8)
project(Prototype1)

#SET(CMAKE_BUILD_TYPE "None")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

find_package(OpenCV REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(src/)
aux_source_directory(src/ SOURCE_FILES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
