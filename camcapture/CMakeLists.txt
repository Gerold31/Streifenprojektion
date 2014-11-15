cmake_minimum_required(VERSION 2.8)
project( camcapture )

find_package( OpenCV REQUIRED )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(./)

aux_source_directory(src/ SOURCE_FILES)

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
