cmake_minimum_required(VERSION 3.10)
project(EasyPackAutoRunCreatorAddon)
set(CMAKE_C_STANDARD 11)

include_directories(PUBLIC ../../include)
add_library(autoruncreator SHARED autorun_creator.c)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
target_link_libraries(autoruncreator "${CMAKE_BINARY_DIR}/easypack.lib")
endif()
install(TARGETS autoruncreator
        LIBRARY DESTINATION lib/easypack/)
