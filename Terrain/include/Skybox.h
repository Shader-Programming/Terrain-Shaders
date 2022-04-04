#pragma once
#include "Camera.h"
#include "Shader.h"

class Skybox {
public:
	Skybox();
	void CreateSkybox();
	void RenderSkybox();
	void LoadSkybox();
	unsigned int facedata;
	Shader shader = Shader("..\\shaders\\Skybox.vs", "..\\shaders\\Skybox.fs");
private:
	std::vector<std::string> faces;
	unsigned int SkyboxVAO;
};