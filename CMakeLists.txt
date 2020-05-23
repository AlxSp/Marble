
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed. Please build the Project in another directory")
endif ()

#The Complete Marble Project
Project(
    MarbleProject
    VERSION 0.1    
)



#if no build type was define set to debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
#Check which build type was initiated and add the appropiate definitions
if(CMAKE_BUILD_TYPE EQUAL Debug)
    add_compile_definitions(MBL_DEBUG)
elseif(CMAKE_BUILD_TYPE EQUAL Release)
    add_compile_definitions(MBL_RELEASE)
elseif(CMAKE_BUILD_TYPE EQUAL Release)
    add_compile_definitions(MBL_DIST)
endif()

#Marble is a c++ 17 library
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(output_dir "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-x86-64")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux)


message(${output_dir} " <- Output dir name")

#Binary target directory of the project
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_subdirectory(Marble)
add_subdirectory(Sandbox)


# install(TARGETS Marble DESTINATION bin)
# install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#   DESTINATION include
# )