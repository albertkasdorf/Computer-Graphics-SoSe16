cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(AntTweakBar
	VERSION 1.16
	LANGUAGES CXX
)

add_library(AntTweakBar
	STATIC
)
add_library(AntTweakBar::AntTweakBar ALIAS AntTweakBar)

target_compile_definitions(AntTweakBar
	PUBLIC _UNIX
)

target_include_directories(AntTweakBar
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/include/AntTweakBar
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(AntTweakBar PUBLIC X11 GL)

target_sources(AntTweakBar
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include/AntTweakBar/AntTweakBar.h
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/LoadOGL.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/LoadOGL.h
	${CMAKE_CURRENT_LIST_DIR}/src/LoadOGLCore.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/LoadOGLCore.h
	${CMAKE_CURRENT_LIST_DIR}/src/MiniGLFW.h
	${CMAKE_CURRENT_LIST_DIR}/src/MiniGLUT.h
	${CMAKE_CURRENT_LIST_DIR}/src/MiniSDL12.h
	${CMAKE_CURRENT_LIST_DIR}/src/MiniSDL13.h
	${CMAKE_CURRENT_LIST_DIR}/src/MiniSFML16.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwBar.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwBar.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwColors.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwColors.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwEventGLFW.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventGLUT.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL12.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL13.c
	${CMAKE_CURRENT_LIST_DIR}/src/TwEventSFML.cpp
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventX11.c
	${CMAKE_CURRENT_LIST_DIR}/src/TwFonts.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwFonts.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwGraph.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwMgr.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwMgr.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwOpenGL.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwOpenGL.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwOpenGLCore.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwOpenGLCore.h
	${CMAKE_CURRENT_LIST_DIR}/src/TwPrecomp.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/TwPrecomp.h
	${CMAKE_CURRENT_LIST_DIR}/src/res/TwXCursors.h
)

SET_SOURCE_FILES_PROPERTIES(
	${CMAKE_CURRENT_LIST_DIR}/src/TwEventGLFW.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventGLUT.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL12.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventSDL13.c
	#${CMAKE_CURRENT_LIST_DIR}/src/TwEventX11.c
	PROPERTIES LANGUAGE CXX
)
