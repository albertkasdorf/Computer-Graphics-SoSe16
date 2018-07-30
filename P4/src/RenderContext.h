#pragma once
#include "ModelMatrix.h"

struct RenderContext
{
	glm::mat4 projection;
	glm::mat4 view;
	ModelMatrix model;
	GLdouble time;
};