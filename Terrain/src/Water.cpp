#include "Water.h"
#include "TextureController.h"

Water::Water() {
	height = 75;
	diameter = 50;
	stepsize = 10;
}

Water::Water(int newheight, int newdiameter, int newstepsize) {
	height = newheight;
	diameter = newdiameter;
	stepsize = newstepsize;
}

void Water::CreatePlane() {
	shader.use();
	for (int y = 0; y < diameter-1; y++) { //Move up creating layers
		float offsety = y * stepsize;
		for (int x = 0; x < diameter - 1; x++) {
			float offsetx = x * stepsize;
			MakeVertex(offsetx, offsety, &vertices);
			MakeVertex(offsetx, offsety+stepsize, &vertices);
			MakeVertex(offsetx+stepsize, offsety, &vertices);
			MakeVertex(offsetx+stepsize, offsety, &vertices);
			MakeVertex(offsetx, offsety+stepsize, &vertices);
			MakeVertex(offsetx+stepsize, offsety+stepsize, &vertices);
		}
	}

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, (vertices.size()*sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);

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
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Water::MakeVertex(int x, int y, std::vector<float>* vertices) {
	//Pos
	vertices->push_back((float)x);
	vertices->push_back((float)height);
	vertices->push_back((float)y);

	//Norms
	vertices->push_back((float)0.0);
	vertices->push_back((float)1.0);
	vertices->push_back((float)0.0);

	//UV
	vertices->push_back((float)x / (diameter * stepsize));
	vertices->push_back((float)y / (diameter * stepsize));
}