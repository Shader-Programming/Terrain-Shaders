#include "Quad.h"

void Quad::RenderQuad(unsigned int& textureobj) {
	shader.use();
	shader.setInt("image", textureobj);
	glActiveTexture(GL_TEXTURE0+textureobj);
	glBindTexture(GL_TEXTURE_2D, textureobj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Quad::CreateQuad() {
	// quad data
	float quadVertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,-1.0f,0.0f,0.0f,0.0f,
		1.0f, 1.0f,0.0f, 1.0f,1.0f,
		1.0f, -1.0f, 0.0f,1.0f,0.0f
	};

	// Create VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}