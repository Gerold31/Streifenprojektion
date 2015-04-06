cmake_minimum_required(VERSION 3.1)
project( tools )

#SET(CMAKE_BUILD_TYPE "None")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

# Set some cmake options
#if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
#endif()
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
endif()

# Set some compiler options
set(CMAKE_CXX_STANDARD "11") # Use C++11

# Get sources
file(GLOB LIBRARY_SOURCES
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"lib/*.cpp" "lib/include/*.hpp" "lib/include/*.h"
)
file(GLOB GETENV_SOURCES
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"getenv/*.cpp" "getenv/*.hpp" "getenv/*.h"
)
file(GLOB COLOR_SOURCES
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"color/*.cpp" "color/*.hpp" "color/*.h"
)

# Create source groups (for Visual Studio)
source_group("Library" FILES ${LIBRARY_SOURCES})
source_group("getenv"  FILES ${GETENV_SOURCES})
source_group("color"   FILES ${COLOR_SOURCES})

# Add binaries
add_library("evalib" SHARED ${LIBRARY_SOURCES})
add_executable("getenv" ${GETENV_SOURCES})
add_executable("color"  ${COLOR_SOURCES})

# Add include directories
include_directories("lib/include")

# Link executable with libraries
target_link_libraries("color" "evalib")
