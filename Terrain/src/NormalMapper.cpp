#include "normalmapper.h"

NormalMapper::NormalMapper() {

}

void NormalMapper::CalculateTanAndBitan(float* vertexdata, int length, unsigned int* indicesdata, int indlength) {
	ExtractVertices(vertexdata, length);
	ComputeTanAndBitan(indicesdata, indlength);
	UnpackVertices();
}

std::vector<float> NormalMapper::GetUpdatedVertexData() {
	return updatedvertexdata;
}

void NormalMapper::UnpackVertices() {
	for (vertex v : vertices) {
		int stride = 14;
		float vertexdata[] = {
			v.pos.x,v.pos.y,v.pos.z,
			v.normal.x,v.normal.y,v.normal.z,
			v.uv.x,v.uv.y,
			v.tan.x,v.tan.y,v.tan.z,
			v.bitan.x,v.bitan.y,v.bitan.z
		};

		for (int i = 0; i < stride; i++) {
			updatedvertexdata.push_back(vertexdata[i]);
		}
	}
}

void NormalMapper::ExtractVertices(float* vertexdata, int length) {
	vertex V;
	for (int i = 0; i < length; i = i + 8) {
		V.pos = glm::vec3(vertexdata[i], vertexdata[i + 1], vertexdata[i + 2]);
		V.normal = glm::vec3(vertexdata[i + 3], vertexdata[i + 4], vertexdata[i + 5]);
		V.uv = glm::vec2(vertexdata[i + 6], vertexdata[i + 7]);
		V.tan = glm::vec3(0.0f, 0.0f, 0.0f);
		V.bitan = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(V);
	}
}

void NormalMapper::ComputeTanAndBitan(unsigned int* indicesdata, int indlength) {
	for (int i = 0; i < indlength; i = i + 3) {
		vertex& a = vertices.at(indicesdata[i]);
		vertex& b = vertices.at(indicesdata[i + 1]);
		vertex& c = vertices.at(indicesdata[i + 2]);

		//edges of triange (delta P)
		glm::vec3 deltapos1 = b.pos - a.pos;
		glm::vec3 deltapos2 = c.pos - a.pos;

		glm::vec2 deltauv1 = b.uv - a.uv;
		glm::vec2 deltauv2 = c.uv - a.uv;

		float r = 1.0f / (deltauv1.x * deltauv2.y - deltauv1.y * deltauv2.x);

		glm::vec3 thistangent = (deltapos1 * deltauv2.y - deltapos2 * deltauv1.y) * r;
		glm::vec3 thisbitangent = (deltapos2 * deltauv1.x - deltapos1 * deltauv2.x) * r;

		a.tan = a.tan + thistangent;
		a.bitan = b.bitan + thisbitangent;

		b.tan = b.tan + thistangent;
		b.bitan = b.bitan + thisbitangent;

		c.tan = c.tan + thistangent;
		c.bitan = c.bitan + thisbitangent;
	}

	for (vertex& v : vertices) {
		v.tan = glm::normalize(v.tan);
		v.bitan = glm::normalize(v.bitan);
	}
}