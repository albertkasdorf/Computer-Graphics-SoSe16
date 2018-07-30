cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(P1
	VERSION 1.0
	LANGUAGES CXX
)

find_package(OpenGL REQUIRED)
find_package(GLUT)

add_executable(P1)

target_sources(P1
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Beschreibung.txt
)

target_link_libraries(P1
	PRIVATE
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARY}
)
