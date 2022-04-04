#include "Skybox.h"
#include "TextureController.h"

Skybox::Skybox() {

}


void Skybox::CreateSkybox() {
    LoadSkybox();

    MakeSkybox(glm::vec3(250, 95, 250), 400 ,&vertices);

    unsigned int VBO;
    glGenVertexArrays(1, & SkyboxVAO);
    glBindVertexArray(SkyboxVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Skybox::RenderSkybox() {
	shader.use();
	glDepthMask(GL_FALSE);
	glBindVertexArray(SkyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, facedata);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDepthMask(GL_TRUE);
}

void Skybox::LoadSkybox() {
	faces.push_back("..\\resources\\Skybox\\Daylight Box_Right.bmp");
    faces.push_back("..\\resources\\Skybox\\Daylight Box_Left.bmp");
    faces.push_back("..\\resources\\Skybox\\Daylight Box_Top.bmp");
    faces.push_back("..\\resources\\Skybox\\Daylight Box_Bottom.bmp");
    faces.push_back("..\\resources\\Skybox\\Daylight Box_Front.bmp");
    faces.push_back("..\\resources\\Skybox\\Daylight Box_Back.bmp");
	facedata = TextureController::LoadCubeMap(faces);
}

void Skybox::MakeSkybox(glm::vec3 planecentre, float offset, std::vector<float>* vertices) {
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset,vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);

    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset, vertices);



    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset, vertices);

    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);



    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);

    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);



    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);

    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);



    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);

    MakeVertex(planecentre.x + offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y + offset, planecentre.z - offset, vertices);



    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);

    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z - offset, vertices);
    MakeVertex(planecentre.x - offset, planecentre.y - offset, planecentre.z + offset, vertices);
    MakeVertex(planecentre.x + offset, planecentre.y - offset, planecentre.z + offset, vertices);
}

void Skybox::MakeVertex(int x, int y, int z, std::vector<float>* vertices) {
    //Pos
    vertices->push_back((float)x);
    vertices->push_back((float)y);
    vertices->push_back((float)z);
}