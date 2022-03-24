#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Shader.h"

class Water {
public:
	Water(int newheight);
	void CreatePlane();
	void RenderPlane(unsigned int& reflection, unsigned int& refraction);
	Shader shader = Shader("..\\shaders\\WaterPlane.vs", "..\\shaders\\WaterPlane.fs");
private:
	float height;
	unsigned int planeVBO, planeVAO, planeEBO;
};