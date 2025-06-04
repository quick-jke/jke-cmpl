cmake_minimum_required(VERSION 3.20)

project(jkecmpl)
add_executable(${PROJECT_NAME} 
    example/main.cpp
    src/lexer.cpp
    src/parser.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
