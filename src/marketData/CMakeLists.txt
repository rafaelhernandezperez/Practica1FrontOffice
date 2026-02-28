add_library(marketData)

target_include_directories(marketData PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ZeroCouponCurve)