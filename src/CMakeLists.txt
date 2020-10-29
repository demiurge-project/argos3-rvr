#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Make sure we are under Unix
#
if(NOT UNIX)
    message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)


# Set up ARGoS compilation information
include_directories(BEFORE ${CMAKE_SOURCE_DIR})

#
# Compile stuff
#
add_subdirectory(plugins)
add_subdirectory(testing)
