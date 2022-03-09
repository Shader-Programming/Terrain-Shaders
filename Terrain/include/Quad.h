#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

class Quad {
public:
	Quad() {};
	void CreateQuad();
	void RenderQuad(unsigned int& textureobj);
private:
	unsigned int quadVBO, quadVAO;
	Shader shader = Shader("..\\shaders\\Quad.vs", "..\\shaders\\Quad.fs");
};