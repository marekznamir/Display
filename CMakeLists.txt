cmake_minimum_required(VERSION 3.10.0)
project(Display VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    Src/Main.cpp
)

if(WIN32)
    add_subdirectory(Inc/pybind11-3.1.0)

    pybind11_add_module(Display MODULE ${SOURCES})

    target_include_directories(Display PRIVATE Inc)

elseif(STM32H7A3xx)
    add_library(Display STATIC &{SOURCES})
    
    target_include_directories(Display PUBLIC Inc)
endif()


