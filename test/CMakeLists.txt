cmake_minimum_required(VERSION 3.24.0)

file(GLOB_RECURSE SRC_FILES ./*.c)
add_executable(c_core_test ${SRC_FILES})

target_include_directories(c_core_test
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_core_test c_core)

set_target_properties(c_json_test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
