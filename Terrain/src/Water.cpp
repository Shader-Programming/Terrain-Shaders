#include "Water.h"
#include "TextureController.h"

Water::Water(int newheight) {
	height = newheight;
}

void Water::CreatePlane() {
	float planesize = 100;
	float vertices[] = {
		-planesize, height, -planesize,	0.0,1.0, 0.0,	0.0,0.0,
		planesize, height, -planesize,	0.0,1.0, 0.0,	1.0,0.0,
		planesize, height, planesize,	0.0,1.0, 0.0,	1.0,1.0,
		-planesize, height, planesize,	0.0,1.0, 0.0,	0.0,1.0
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

void Water::RenderPlane() {
	shader.use();
	glm::mat4 model = glm::mat4(1.0);
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}