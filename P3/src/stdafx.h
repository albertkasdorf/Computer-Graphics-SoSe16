// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>

// Reference additional headers your program requires here
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <memory>
#include <cassert>
#include <fstream>
#include <functional>
#include <map>
#include <random>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "IScene.h"

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "SOIL.lib" )
