cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(P4
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

add_executable(P4)

target_compile_features(P4
	PRIVATE cxx_std_14
)

target_sources(P4
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/Beschreibung.txt
	${CMAKE_CURRENT_LIST_DIR}/src/Camera.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Camera.h
	${CMAKE_CURRENT_LIST_DIR}/src/File.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/File.h
	${CMAKE_CURRENT_LIST_DIR}/src/IHierarchyRenderer.h
	${CMAKE_CURRENT_LIST_DIR}/src/IScene.h
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ModelMatrix.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ModelMatrix.h
	${CMAKE_CURRENT_LIST_DIR}/src/NVidiaOptimus.h
	${CMAKE_CURRENT_LIST_DIR}/src/PhongShaderProgram.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/PhongShaderProgram.h
	${CMAKE_CURRENT_LIST_DIR}/src/PlanetSolarSystemObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/PlanetSolarSystemObject.h
	${CMAKE_CURRENT_LIST_DIR}/src/RenderContext.h
	${CMAKE_CURRENT_LIST_DIR}/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Scene.h
	${CMAKE_CURRENT_LIST_DIR}/src/ShaderProgram.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ShaderProgram.h
	${CMAKE_CURRENT_LIST_DIR}/src/SkyBox.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/SkyBox.h
	${CMAKE_CURRENT_LIST_DIR}/src/SolarSystemObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/SolarSystemObject.h
	${CMAKE_CURRENT_LIST_DIR}/src/SunShaderProgram.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/SunShaderProgram.h
	${CMAKE_CURRENT_LIST_DIR}/src/SunSolarSystemObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/SunSolarSystemObject.h
	${CMAKE_CURRENT_LIST_DIR}/src/Texture.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/Texture.h
	${CMAKE_CURRENT_LIST_DIR}/src/VertexArrayObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/VertexArrayObject.h
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos.3ds
	#${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos.obj
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Deimos_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Distortion1_512x512.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Distortion1_2560x2560.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Distortion2_256x256.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Distortion3_800x800.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Distortion4_200x200.jpg
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
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Sun_ColorMap2.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Uranus_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/Venus_ColorMap.jpg
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/ame_starfield.shader
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/readme
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_bk.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_dn.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_ft.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_lf.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_rt.png
	${CMAKE_CURRENT_LIST_DIR}/src/Assets/ame_starfield/starfield_up.png
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Model.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Model.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Phong.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Phong.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Simple.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Simple.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/SkyBox.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/SkyBox.vert
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Sun.frag
	${CMAKE_CURRENT_LIST_DIR}/src/Shader/Sun.vert
)

target_link_libraries(P4
	PRIVATE
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLFW3_LIBRARY}
	${SOIL_LIBRARIES}
	${ASSIMP_LIBRARIES}
)
