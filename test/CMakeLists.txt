include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../structures)

file(GLOB test_sources "*.cpp")

add_executable(runTests ${test_sources})
target_link_libraries(runTests gtest gtest_main ${library})