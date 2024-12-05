cmake_minimum_required(VERSION 3.10)

project(EasyPack)

set(CMAKE_C_STANDARD 11)

add_executable(size_initializer build_utilities/size_initializer.c)

add_executable(easypack src/main.c src/file_sections.c src/embedded_filesystem.c src/utils.c src/string_array.c src/forward.c)
target_include_directories(easypack PUBLIC include)

add_dependencies(easypack size_initializer)
add_custom_command(
	TARGET easypack POST_BUILD
	COMMAND mkdir -p awd/dwa
	COMMAND echo "Hello World" > awd/dwa/hi
	COMMAND ./size_initializer easypack)


enable_testing()

add_executable(test_string_array tests/string_array.c src/string_array.c)
target_include_directories(test_string_array PUBLIC include)
add_test(NAME test_string_array
          COMMAND test_string_array)
