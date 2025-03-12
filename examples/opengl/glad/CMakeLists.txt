cmake_minimum_required(VERSION 3.12)

project(glad
	VERSION 0.1.36
	DESCRIPTION "OpenGL Loader based on the official specifications"
	HOMEPAGE_URL "https://glad.dav1d.de/"
	LANGUAGES CXX
	)

set(GLAD_SOURCES
	${PROJECT_SOURCE_DIR}/include/glad/glad.h
	${PROJECT_SOURCE_DIR}/src/glad.c
	)

add_library(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_sources(${PROJECT_NAME} PUBLIC ${GLAD_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})

