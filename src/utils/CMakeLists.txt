add_library(utils INTERFACE)

target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(DayCount)
add_subdirectory(GoalSeeker)