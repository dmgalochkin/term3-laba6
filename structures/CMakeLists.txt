file(GLOB headers "*.hpp")
file(GLOB sources "*.cpp")

# Create header-only library if no source files exist
if(NOT sources)
    add_library(${library} INTERFACE)
    target_include_directories(${library} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
else()
    add_library(${library} STATIC ${sources} ${headers})
endif()