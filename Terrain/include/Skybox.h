#pragma once
#include "Camera.h"
#include "Shader.h"

class Skybox {
public:
	Skybox();
	void CreateSkybox();
	void RenderSkybox();
	void LoadSkybox();
	void MakeSkybox(glm::vec3 planecentre, float offset, std::vector<float>* vertices);
	void MakeVertex(int x, int y, int z, std::vector<float>* vertices);
	unsigned int facedata;
	Shader shader = Shader("..\\shaders\\Skybox.vs", "..\\shaders\\Skybox.fs");
private:
	std::vector<std::string> faces;
	std::vector<float> vertices;
	unsigned int SkyboxVAO;
};