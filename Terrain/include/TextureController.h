#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <string>

class TextureController {
public:
	static unsigned int LoadTexture(char const* path);
	static void AssignTexture(unsigned int texture, Shader shader, std::string name);
	static unsigned int CreateTexture(int width, int height);
	static void CreateFBOCA(unsigned int& colourattatchment, int SCR_WIDTH, int SCR_HEIGHT);
	static void CreateFBODA(unsigned int depthattatchment, int SCR_WIDTH, int SCR_HEIGHT);
};