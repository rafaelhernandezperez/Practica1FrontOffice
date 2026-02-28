add_library(utils)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(DayCount)
add_subdirectory(GoalSeeker)