cmake_minimum_required(VERSION 4.1 FATAL_ERROR)

project(boostTraining LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Прямые пути для Boost 1.89.0
set(BOOST_INCLUDE_DIR "C:/local/boost_1_89_0")
set(BOOST_LIB_DIR "C:/local/boost_1_89_0/lib")

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${BOOST_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${BOOST_LIB_DIR})

# Используем упрощенные имена библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE 
    C:/local/boost_1_89_0/lib32-msvc-14.1/libboost_filesystem-vc141-mt-sgd-x32-1_89.lib
)
