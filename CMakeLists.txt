cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME term3-laba6)
project(${PROJECT_NAME})

set(application main)
set(library structures)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config" FORCE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/structures)

add_subdirectory(main)
add_subdirectory(structures)
add_subdirectory(gtest)
add_subdirectory(test)