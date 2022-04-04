#include "Skybox.h"
#include "TextureController.h"

Skybox::Skybox() {

}


void Skybox::CreateSkybox() {
    LoadSkybox();
    int skyboxsize = 100;
    float vertices[] = {
        // positions          
        -skyboxsize,  skyboxsize, -skyboxsize,
        -skyboxsize, -skyboxsize, -skyboxsize,
         skyboxsize, -skyboxsize, -skyboxsize,
         skyboxsize, -skyboxsize, -skyboxsize,
         skyboxsize,  skyboxsize, -skyboxsize,
        -skyboxsize,  skyboxsize, -skyboxsize,

        -skyboxsize, -skyboxsize,  skyboxsize,
        -skyboxsize, -skyboxsize, -skyboxsize,
        -skyboxsize,  skyboxsize, -skyboxsize,
        -skyboxsize,  skyboxsize, -skyboxsize,
        -skyboxsize,  skyboxsize,  skyboxsize,
        -skyboxsize, -skyboxsize,  skyboxsize,

         skyboxsize, -skyboxsize, -skyboxsize,
         skyboxsize, -skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize, -skyboxsize,
         skyboxsize, -skyboxsize, -skyboxsize,

        -skyboxsize, -skyboxsize,  skyboxsize,
        -skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize, -skyboxsize,  skyboxsize,
        -skyboxsize, -skyboxsize,  skyboxsize,

        -skyboxsize,  skyboxsize, -skyboxsize,
         skyboxsize,  skyboxsize, -skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
         skyboxsize,  skyboxsize,  skyboxsize,
        -skyboxsize,  skyboxsize,  skyboxsize,
        -skyboxsize,  skyboxsize, -skyboxsize,

        -skyboxsize, -skyboxsize, -skyboxsize,
        -skyboxsize, -skyboxsize,  skyboxsize,
         skyboxsize, -skyboxsize, -skyboxsize,
         skyboxsize, -skyboxsize, -skyboxsize,
        -skyboxsize, -skyboxsize,  skyboxsize,
         skyboxsize, -skyboxsize,  skyboxsize
    };

    unsigned int VBO;
    glGenVertexArrays(1, & SkyboxVAO);
    glBindVertexArray(SkyboxVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Skybox::RenderSkybox() {
	shader.use();
	glDepthMask(GL_FALSE);
	glBindVertexArray(SkyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, facedata);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void Skybox::LoadSkybox() {
	faces.push_back("..\\resources\\cursedsteve.png");
	faces.push_back("..\\resources\\cursedsteve.png");
	faces.push_back("..\\resources\\cursedsteve.png");
	faces.push_back("..\\resources\\cursedsteve.png");
	faces.push_back("..\\resources\\cursedsteve.png");
	faces.push_back("..\\resources\\cursedsteve.png");
	facedata = TextureController::LoadCubeMap(faces);
}