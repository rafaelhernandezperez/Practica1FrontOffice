add_library(marketData INTERFACE)

target_include_directories(marketData INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ZeroCouponCurve)
add_subdirectory(Index)