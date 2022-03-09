#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <random>

#include "Shader.h"
#include "Camera.h"

#define SIZE 1024*100

struct particle {
	glm::vec4 pos;
	glm::vec4 velocities;
	glm::vec4 col;
};