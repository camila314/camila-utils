cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(camila-utils LANGUAGES CXX)

if(NOT COMMAND CPMAddPackage)
    message(FATAL_ERROR "CPM not found! Please include this library after including Geode.")
endif()

CPMAddPackage("gh:camila314/UIBuilder#main")
CPMAddPackage("gh:camila314/cpp-reactive#5b5ae3e")

add_library(camila-utils INTERFACE)

target_include_directories(camila-utils INTERFACE include)
target_link_libraries(camila-utils INTERFACE UIBuilder cpp-reactive geode-sdk)
