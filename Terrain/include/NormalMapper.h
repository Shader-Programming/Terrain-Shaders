#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

struct vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 bitan;
};

class NormalMapper {
public:
	NormalMapper();
	void CalculateTanAndBitan(float* vertexdata, int length, unsigned int* indicesdata, int indlength);
	std::vector<float> GetUpdatedVertexData();
private:
	std::vector<vertex> vertices;
	void UnpackVertices();
	void ExtractVertices(float* vertexdata, int length);
	void ComputeTanAndBitan(unsigned int* indicesdata, int indlength);
	std::vector<float> updatedvertexdata;
};