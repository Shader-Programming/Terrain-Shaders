#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>

class TextureController {
public:
	static unsigned int LoadTexture(char const* path);
	static void AssignTexture(unsigned int texture, int textureid, Shader shader, std::string name);
	static unsigned int CreateTexture(int width, int height);
	static unsigned int CreateFBOCA(unsigned int FBO, int SCR_WIDTH, int SCR_HEIGHT, int FBOid);
};