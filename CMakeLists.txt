cmake_minimum_required(VERSION 3.10.0)
project(Display VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE DISPLAY_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c
)

set(SOURCES
    ${DISPLAY_SOURCES}
)

add_subdirectory(Inc/Extern/u8g2-2.37.1)

if(WIN32)
    add_subdirectory(Inc/Extern/pybind11-3.1.0)

    pybind11_add_module(Display MODULE ${SOURCES})

    target_include_directories(Display PRIVATE Inc)

elseif(STM32H7A3xx)
    add_library(Display STATIC ${SOURCES})
    
    target_include_directories(Display PUBLIC Inc)
    
    target_link_libraries(Display PRIVATE stm32cubemx)
endif()

target_link_libraries(Display PUBLIC u8g2)
