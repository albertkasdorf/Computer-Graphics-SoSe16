cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(P5 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
find_package(ASSIMP REQUIRED)

add_subdirectory(3rdParty/AntTweakBar)

add_executable(P5)

target_compile_features(P5
	PRIVATE cxx_std_14)

target_compile_definitions(P5
	PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_sources(P5
	PRIVATE
	src/Camera.cpp
	src/Camera.h
	src/ElapsedTime.cpp
	src/ElapsedTime.h
	src/FPSCounter.cpp
	src/FPSCounter.h
	src/Main.cpp
	src/Model.cpp
	src/Model.h
	src/NVidiaOptimus.h
	src/Rescale.cpp
	src/Rescale.h
	src/StopWatch.cpp
	src/StopWatch.h
	src/Assimp/Assimp.h
	src/GL/Buffer.cpp
	src/GL/Buffer.h
	src/GL/ShaderProgram.cpp
	src/GL/ShaderProgram.h
	src/GL/Texture.cpp
	src/GL/Texture.h
	src/GL/VertexArray.cpp
	src/GL/VertexArray.h
	src/IO/Directory.cpp
	src/IO/Directory.h
	src/IO/File.cpp
	src/IO/File.h
	src/Renderer/CoordinateAxis.cpp
	src/Renderer/CoordinateAxis.h
	src/Renderer/Floor.cpp
	src/Renderer/Floor.h
	src/Scene/IScene.h
	src/Scene/Manager.cpp
	src/Scene/Manager.h
	src/Scene/Scene.cpp
	src/Scene/Scene.h
	src/Scene/Simple.cpp
	src/Scene/Simple.h
	src/UI/PropertyWindow.cpp
	src/UI/PropertyWindow.h)

target_link_libraries(P5
	PRIVATE
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLFW3_LIBRARY}
	${SOIL_LIBRARIES}
	${ASSIMP_LIBRARIES}
	AntTweakBar)
