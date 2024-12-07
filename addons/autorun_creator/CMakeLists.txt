cmake_minimum_required(VERSION 3.10)
project(EasyPackAutoRunCreatorAddon)
set(CMAKE_C_STANDARD 11)

include_directories(PUBLIC ../../include)
add_library(autoruncreator SHARED autorun_creator.c)
install(TARGETS autoruncreator
        LIBRARY DESTINATION lib)
