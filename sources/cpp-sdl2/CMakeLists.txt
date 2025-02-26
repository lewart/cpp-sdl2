cmake_minimum_required(VERSION 3.12)

project(cpp_sdl2
	VERSION 0.1.0
	DESCRIPTION "Basic C++17 bindings to SDL2"
	HOMEPAGE_URL "https://github.com/Edhebi/cpp-sdl2"
	LANGUAGES C CXX
	)

set(CPP_SDL2_SOURCES
	${PROJECT_SOURCE_DIR}/color.hpp
	${PROJECT_SOURCE_DIR}/event.hpp
	${PROJECT_SOURCE_DIR}/exception.hpp
	${PROJECT_SOURCE_DIR}/game_controller.hpp
	${PROJECT_SOURCE_DIR}/haptic.hpp
	${PROJECT_SOURCE_DIR}/joystick.hpp
	${PROJECT_SOURCE_DIR}/mouse.hpp
	${PROJECT_SOURCE_DIR}/pixel.hpp
	${PROJECT_SOURCE_DIR}/rect.hpp
	${PROJECT_SOURCE_DIR}/renderer.hpp
	${PROJECT_SOURCE_DIR}/sdl.hpp
	${PROJECT_SOURCE_DIR}/shared_object.hpp
	${PROJECT_SOURCE_DIR}/simd.hpp
	${PROJECT_SOURCE_DIR}/surface.hpp
	${PROJECT_SOURCE_DIR}/texture.hpp
	${PROJECT_SOURCE_DIR}/timer.hpp
	${PROJECT_SOURCE_DIR}/utils.hpp
	${PROJECT_SOURCE_DIR}/vec2.hpp
	${PROJECT_SOURCE_DIR}/window.hpp
	)

add_library(cpp_sdl2 INTERFACE)
target_compile_features(cpp_sdl2 INTERFACE cxx_std_17)
target_sources(cpp_sdl2 INTERFACE ${CPP_SDL2_SOURCES})
target_include_directories(cpp_sdl2 INTERFACE ${PROJECT_SOURCE_DIR})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(cpp_sdl2 INTERFACE SDL2::SDL2)

option(CPP_SDL2_ENABLE_OPENGL "Enable opengl functionalities for windows." OFF)
if(CPP_SDL2_ENABLE_OPENGL)
	find_package(OpenGL REQUIRED)
	target_link_libraries(cpp_sdl2 INTERFACE OpenGL::GL)
	target_compile_definitions(cpp_sdl2 INTERFACE CPP_SDL2_ENABLE_OPENGL)
endif()

option(CPP_SDL2_ENABLE_VULKAN "Enable vulkan functionalities for windows." OFF)
if(CPP_SDL2_ENABLE_VULKAN)
	find_package(Vulkan REQUIRED)
	target_link_libraries(cpp_sdl2 INTERFACE Vulkan::Vulkan)
	target_compile_definitions(cpp_sdl2 INTERFACE CPP_SDL2_ENABLE_VULKAN)
endif()

option(CPP_SDL2_ENABLE_SDL_IMAGE "Use SDL_Image to provide image loading tools" OFF)
if(CPP_SDL2_ENABLE_SDL_IMAGE)
	if(NOT TARGET SDL2::SDL2_image)
		find_package(sdl2-image CONFIG REQUIRED)
	endif()
	target_link_libraries(cpp_sdl2 INTERFACE SDL2::SDL2_image)
	target_compile_definitions(cpp_sdl2 INTERFACE CPP_SDL2_ENABLE_SDL_IMAGE)
endif()

option(CPP_SDL2_DISABLE_EXCEPTIONS "Disable exceptions for cpp-sdl2" OFF)
if(CPP_SDL2_DISABLE_EXCEPTIONS)
	target_compile_definitions(cpp_sdl2 INTERFACE CPP_SDL2_DISABLE_EXCEPTIONS)
endif()

