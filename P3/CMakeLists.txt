cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(P3
	VERSION 1.0
	LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
find_package(ASSIMP REQUIRED)

add_executable(P3)

target_compile_features(P3
	PRIVATE cxx_std_14)

target_sources(P3
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/Beschreibung.txt
	${CMAKE_CURRENT_LIST_DIR}/src/Camera.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Camera.h
	${CMAKE_CURRENT_LIST_DIR}/src/File.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/File.h
	${CMAKE_CURRENT_LIST_DIR}/src/IScene.h
	${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ModelMatrix.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ModelMatrix.h
	${CMAKE_CURRENT_LIST_DIR}/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Scene.h
	${CMAKE_CURRENT_LIST_DIR}/src/ShaderProgram.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ShaderProgram.h
	${CMAKE_CURRENT_LIST_DIR}/src/stdafx.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/stdafx.h
	${CMAKE_CURRENT_LIST_DIR}/src/Texture.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Texture.h
	${CMAKE_CURRENT_LIST_DIR}/src/VertexArrayObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/VertexArrayObject.h
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos.3ds
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos.obj
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Earth_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Jupiter_ColorMap1.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Jupiter_ColorMap2.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Mars_ColorMap1.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Mars_ColorMap2.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Mercury_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Moon_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Neptune_ColorMap.jpg
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/Phobos.3ds
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Phobos_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Phobos_ColorMap.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Pluto_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Saturn_ColorMap.jpg
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/sphere_high.obj
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/sphere_low.obj
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Sun_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Uranus_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Venus_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Coord.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Coord.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/default330.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/default330.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Model.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Model.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Simple.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Simple.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Sun.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Sun.vert
)

target_link_libraries(P3
	PRIVATE
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLFW3_LIBRARY}
	${SOIL_LIBRARIES}
	${ASSIMP_LIBRARIES})
