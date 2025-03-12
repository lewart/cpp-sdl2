cmake_minimum_required(VERSION 3.25)

project(cpp_sdl2_example)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
add_subdirectory(../sources/cpp-sdl2 ../sources/cpp-sdl2/build)

add_executable(${PROJECT_NAME}_general general/main.cpp general/other_unit.cpp)
target_link_libraries(cpp_sdl2_example_general PRIVATE cpp_sdl2 SDL2::SDL2)

target_include_directories(cpp_sdl2_example_general PRIVATE
                           "${PROJECT_SOURCE_DIR}/general"
                           "${PROJECT_SOURCE_DIR}/../sources/cpp-sdl2"
                           )

#add_library(cpp_sdl2_example_dll_shared SHARED dll/dll.c)
#target_link_libraries(cpp_sdl2_example_dll_shared PRIVATE cpp_sdl2)
#add_executable(cpp_sdl2_example_dll dll/main.cpp)
#target_link_libraries(cpp_sdl2_example_dll PRIVATE cpp_sdl2_example_dll_shared cpp_sdl2 SDL2::SDL2main)

if(CPP_SDL2_ENABLE_OPENGL)
	add_executable(${PROJECT_NAME}_opengl opengl/main.cpp opengl/glad/src/glad.c)
	add_subdirectory(opengl/glad opengl/glad/build)
	target_include_directories(cpp_sdl2_example_opengl PRIVATE opengl/glad/include)
	target_link_libraries(cpp_sdl2_example_opengl PRIVATE cpp_sdl2 SDL2::SDL2 glad)
endif()

if(CPP_SDL2_ENABLE_VULKAN)
	add_executable(${PROJECT_NAME}_vulkan vulkan/main.cpp)
	target_sources(cpp_sdl2_example_vulkan PRIVATE
		vulkan/simple_tri.frag.glsl
		vulkan/simple_tri.frag.spv
		vulkan/simple_tri.vert.glsl
		vulkan/simple_tri.vert.spv
		)
	target_link_libraries(cpp_sdl2_example_vulkan PRIVATE cpp_sdl2 SDL2::SDL2)
endif()
