#include "TextureController.h"
#include <stdlib.h>

unsigned int TextureController::LoadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum intformat;
		GLenum format;
		if (nrComponents == 1) {
			intformat = GL_R32F;
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			intformat = GL_RGB32F;
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			intformat = GL_RGBA32F;
			format = GL_RGBA;
		}

		std::cout << "Image loaded with " << nrComponents << " components" << std::endl;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, intformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		std::cout << "Loaded texture at path " << path <<  " with ID: " << textureID << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int TextureController::LoadCubeMap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data) {

			GLenum intformat;
			GLenum format;
			if (nrComponents == 1) {
				intformat = GL_R32F;
				format = GL_RED;
			}
			else if (nrComponents == 3) {
				intformat = GL_RGB32F;
				format = GL_RGB;
			}
			else if (nrComponents == 4) {
				intformat = GL_RGBA32F;
				format = GL_RGBA;
			}

			std::cout << "Image loaded with " << nrComponents << " components" << std::endl;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, intformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Loaded texture at path " << faces[i] << " with ID: " << textureID << std::endl;
		}
		else {
			std::cout << "Texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void TextureController::AssignTexture(unsigned int texture, Shader& shader, std::string name) {
	shader.use();
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.setInt(name, texture);
	std::cout << "Assigned texture with ID " << texture <<  " to shader" <<std::endl;
}

unsigned int TextureController::CreateTexture(int width, int height) {
	//CHANGED TO FLOATS
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	std::cout << "Created blank texture with ID: " << textureID << std::endl;
	return textureID;
}

void TextureController::CreateFBOCA(unsigned int& colourattatchment, int SCR_WIDTH, int SCR_HEIGHT) {
	glGenTextures(1, &colourattatchment);
	glBindTexture(GL_TEXTURE_2D, colourattatchment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourattatchment, 0);
	std::cout << "Created FBO colour attatchment with ID: " << colourattatchment << std::endl;
}

void TextureController::CreateFBODA(unsigned int depthattatchment, int SCR_WIDTH, int SCR_HEIGHT) {
	glGenTextures(1, &depthattatchment);
	glBindTexture(GL_TEXTURE_2D, depthattatchment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthattatchment, 0);
	std::cout << "Created FBO depth attatchment with ID: " << depthattatchment << std::endl;
}