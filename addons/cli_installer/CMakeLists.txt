cmake_minimum_required(VERSION 3.10)
project(EasyPackCLIInstallerAddon)
set(CMAKE_C_STANDARD 11)

include_directories(PUBLIC ../../include)
#add_executable(installer installer.c)
add_library(cliinstaller SHARED cli_installer.c)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
target_link_libraries(cliinstaller "${CMAKE_BINARY_DIR}/easypack.lib")
endif()
install(TARGETS cliinstaller
        LIBRARY DESTINATION lib/easypack/)
