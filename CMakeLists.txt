cmake_minimum_required(VERSION 3.10)

project(EasyPack)

set(CMAKE_C_STANDARD 11)

add_executable(size_initializer build_utilities/size_initializer.c)

add_executable(easypack src/main.c src/file_sections.c)
target_include_directories(easypack PUBLIC include)

add_dependencies(easypack size_initializer)
add_custom_command(
	TARGET easypack POST_BUILD
	COMMAND ./size_initializer easypack)
