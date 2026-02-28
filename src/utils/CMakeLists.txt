add_library(instruments)

target_include_directories(instruments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Bond)
add_subdirectory(Swap)
add_subdirectory(Factory)