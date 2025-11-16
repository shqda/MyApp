cmake_minimum_required(VERSION 3.30)
project(MyApp)

set(CMAKE_CXX_STANDARD 23)

add_library(sqlite3 STATIC external/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC external/sqlite)

add_executable(myApp main.cpp
        DataBase.cpp
        DataBase.h
        Employee.cpp
        Employee.h
        DataGenerator.cpp
        DataGenerator.h
)

target_link_libraries(myApp sqlite3)
