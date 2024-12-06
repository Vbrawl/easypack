cmake_minimum_required(VERSION 3.10)

project(EasyPackExampleAddon)

set(CMAKE_C_STANDARD 11)

include_directories(PUBLIC ../include)

add_library(example SHARED example_addon.c)
