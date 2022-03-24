#include "Water.h"
#include "TextureController.h"

Water::Water(int newheight) {
	height = newheight;
}

void Water::CreatePlane() {
	float planesize = 240;
	float planeoffset = 250;
	float vertices[] = {
		-planesize + planeoffset - 10, height, -planesize + planeoffset - 10,  0.0,1.0, 0.0,	0.0,0.0,
		planesize + planeoffset, height, -planesize + planeoffset - 10,	0.0,1.0, 0.0,	1.0,0.0, 
		planesize + planeoffset, height, planesize + planeoffset,	0.0,1.0, 0.0,	1.0,1.0, 
		-planesize + planeoffset - 10, height, planesize + planeoffset,	0.0,1.0, 0.0,	0.0,1.0
	};

	unsigned int indices[] = {
	3,2,1,
	3,1,0
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Water::RenderPlane(unsigned int& reflection, unsigned int& refraction) {
	shader.use();
	shader.setInt("reflection", reflection);
	shader.setInt("refraction", refraction);
	glActiveTexture(GL_TEXTURE0 + reflection);
	glBindTexture(GL_TEXTURE_2D, reflection);
	glActiveTexture(GL_TEXTURE0 + refraction);
	glBindTexture(GL_TEXTURE_2D, refraction);
	glm::mat4 model = glm::mat4(1.0);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}