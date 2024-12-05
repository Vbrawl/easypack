cmake_minimum_required(VERSION 3.10)

project(EasyPack)

set(CMAKE_C_STANDARD 11)

add_executable(size_initializer build_utilities/size_initializer.c)

add_executable(easypack src/main.c src/file_sections.c src/embedded_filesystem.c src/utils.c src/string_array.c src/forward.c)
target_include_directories(easypack PUBLIC include)

add_dependencies(easypack size_initializer)
add_custom_command(
  TARGET easypack POST_BUILD
  COMMENT "Initializing easypack size variable"
  COMMAND ./size_initializer easypack)

execute_process(
  COMMAND cp build_utilities/wrap_awd.sh ${CMAKE_BINARY_DIR}/wrap_awd.sh
  COMMAND mkdir -p ${CMAKE_BINARY_DIR}/awd/dwa
  COMMAND bash -c "echo \"Hello World\" > ${CMAKE_BINARY_DIR}/awd/dwa/hi"
)

enable_testing()

macro(TESTCASE test_name)
add_executable(${test_name} ${ARGN}) 
target_compile_options(${test_name} PRIVATE -fsanitize=address -g)
target_link_options(${test_name} PRIVATE -fsanitize=address)
target_include_directories(${test_name} PUBLIC include)
add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

TESTCASE(test_string_array tests/string_array.c src/string_array.c)
