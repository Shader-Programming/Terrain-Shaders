#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Shader.h"

class Water {
public:
	Water();
	Water(int newheight, int newdiameter, int newstepsize);
	void CreatePlane();
	void RenderPlane(unsigned int& reflection, unsigned int& refraction);
	Shader shader = Shader("..\\shaders\\WaterPlane.vs", "..\\shaders\\WaterPlane.fs");
private:
	float height;
	float diameter;
	float stepsize;
	std::vector<float> vertices;
	void MakeVertex(int x, int y, std::vector<float>* vertices);
	unsigned int planeVBO, planeVAO;
};