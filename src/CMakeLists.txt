cmake_minimum_required(VERSION 3.27)
project(practica_cpp)

set(CMAKE_CXX_STANDARD 17)

# Buscar Boost
find_package(Boost REQUIRED)

# Incluir los headers de Boost
include_directories(${Boost_INCLUDE_DIRS})

# Tu ejecutable
add_executable(practica_cpp main.cpp)

# Enlazar Boost
target_link_libraries(practica_cpp ${Boost_LIBRARIES})