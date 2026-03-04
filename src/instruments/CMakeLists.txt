add_library(instruments INTERFACE)

target_include_directories(instruments INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Bond)
add_subdirectory(Swap)
add_subdirectory(Factory)