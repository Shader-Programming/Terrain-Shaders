#include "TextureController.h"

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

		//std::cout << "Image loaded with " << nrComponents << " components" << std::endl;

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

void TextureController::AssignTexture(unsigned int texture, int textureid, Shader shader, std::string name) {
	shader.use();
	shader.setInt(name, textureid);
	glActiveTexture(GL_TEXTURE0 + textureid);
	glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int TextureController::CreateTexture(int width, int height) {
	//CHANGED TO FLOATS
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	std::cout << "Created texture with ID: " << textureID << std::endl;
	return textureID;
}

unsigned int TextureController::CreateFBOCA(unsigned int FBO, int SCR_WIDTH, int SCR_HEIGHT, int FBOid) {
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	unsigned int attatchment = CreateTexture(SCR_WIDTH, SCR_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+FBOid, GL_TEXTURE_2D, attatchment, 0);
	return attatchment;
}