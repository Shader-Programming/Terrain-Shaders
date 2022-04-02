#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

#include <Model.h>
#include "Terrain.h"
#include "TextureController.h"
#include "Quad.h"
#include "Water.h"

#include<string>
#include <iostream>
#include <numeric>


const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

//FrameBuffers
unsigned int MountainFBO, WaterFBO;
unsigned int mountainCA, mountainDA, waterCA, waterDA;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void SetTerrainUniforms(Shader& shader);
void SetContinuousUniforms(Shader& terrain, Shader& water);
void SetFBOColour();
void CreateFBO(unsigned int& FBO, unsigned int& colourattatchment, unsigned int& depthattatchment);

// camera
Camera camera(glm::vec3(260,100,300));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//arrays
unsigned int terrainVAO;

unsigned int heightmap;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Maps
	unsigned int noisetexture = TextureController::CreateTexture(1024, 1024);
	unsigned int normalmap = TextureController::CreateTexture(1024, 1024);

	//Noise Generation
	Shader computenoise("..\\Shaders\\ComputeNoise.cms");
	computenoise.use();
	glBindImageTexture(0, noisetexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((GLuint)64, (GLuint)16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//Normal Generation
	Shader computecdm("..\\Shaders\\ComputeCDM.cms");
	computecdm.use();
	computecdm.setInt("scale", 100);
	TextureController::AssignTexture(noisetexture, computecdm, "noisemap");
	glBindImageTexture(0, normalmap, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((GLuint)64, (GLuint)16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//Terrain Constructor ; number of grids in width, number of grids in height, gridSize
	Terrain terrain(50, 50,10);
	terrain.AssignTerrainTextures("..\\Resources\\rock\\diffuse.jpg", "..\\Resources\\grass\\diffuse.jpg");
	TextureController::AssignTexture(noisetexture, terrain.shader, "noise");
	terrainVAO = terrain.getVAO();
	terrain.heightmap = noisetexture;

	//Quad for 2D
	Quad quad;
	quad.CreateQuad();

	//Water Plane
	Water water(75,50,10);
	water.CreatePlane();
	
	water.shader.use();
	unsigned int waterNormals = TextureController::LoadTexture("..\\Resources\\water\\normal.png");
	unsigned int waterDuDv = TextureController::LoadTexture("..\\Resources\\water\\dudv.png");

	TextureController::AssignTexture(waterNormals, water.shader, "normalmap");
	TextureController::AssignTexture(waterDuDv, water.shader, "DuDv");
	
	water.shader.setVec3("lightdir", glm::vec3(-1.0f, -1.0f, -1.0f));

	SetTerrainUniforms(terrain.shader);

	//Clip Plane
	glEnable(GL_CLIP_DISTANCE0);

	CreateFBO(MountainFBO,mountainCA,mountainDA);
	CreateFBO(WaterFBO, waterCA, waterDA);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glm::vec3 storedpos = camera.Position;
		float storedpitch = camera.Pitch;

		//Set Global Frame Uniforms
		SetContinuousUniforms(terrain.shader, water.shader);

		//REFLECTION
		terrain.shader.use();
		glm::vec4 plane = glm::vec4(0, 1, 0, -75);
		terrain.shader.setVec4("clipplane", plane);

		glBindFramebuffer(GL_FRAMEBUFFER, MountainFBO);
		//Clean back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enable Depth
		glEnable(GL_DEPTH_TEST);
		//Render Scene to fill FBO
		camera.Position = glm::vec3(storedpos.x, 70, storedpos.z);
		camera.Pitch = storedpitch * -1;
		terrain.RenderTerrain();

		//Reset Camera
		camera.Position = glm::vec3(storedpos.x, storedpos.y, storedpos.z);
		camera.Pitch = storedpitch;

		//REFRACTION
		terrain.shader.use();
		plane = glm::vec4(0, -1, 0, 75);
		terrain.shader.setVec4("clipplane", plane);

		glBindFramebuffer(GL_FRAMEBUFFER, WaterFBO);
		//Clean back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enable Depths
		glEnable(GL_DEPTH_TEST);
		//Render Scene to fill FBO
		terrain.RenderTerrain();




		//Now use default FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Clean back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enable Depth
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CLIP_DISTANCE0);
		//Render Scene to fill FBO
		glEnable(GL_CULL_FACE);
		//Render Everything
		terrain.RenderTerrain();
		glDisable(GL_CULL_FACE);
		
		//Water Uniforms
		water.shader.use();
		water.shader.setVec3("camerapos", camera.Position);
		water.shader.setFloat("screenW", SCR_WIDTH);
		water.shader.setFloat("screenH", SCR_HEIGHT);
		water.shader.setFloat("time", glfwGetTime());
		water.RenderPlane(mountainCA,waterCA);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}





// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void SetTerrainUniforms(Shader& shader) {
	shader.use();

	const float red = 0.53;
	const float green = 0.81;
	const float blue = 0.92;

	glClearColor(red, green, blue, 1.0);
	//light properties
	shader.setVec3("dirlight.direction", glm::vec3(-1.0, -1.0, -1.0));
	shader.setVec3("dirlight.ambient", 0.6f, 0.6f, 0.6f);
	shader.setVec3("dirlight.diffuse", 0.55f, 0.55f, 0.55f);
	shader.setVec3("dirlight.specular", 0.6f, 0.6f, 0.6f);
	//material properties
	shader.setVec3("mat.ambient", 0.5, 0.5, 0.5);
	shader.setVec3("mat.diffuse", 0.396, 0.741, 0.691);
	shader.setVec3("mat.specular", 0.297f, 0.308f, 0.306f);
	shader.setFloat("mat.shininess", 0.9f);

	shader.setFloat("scale", 100);
	shader.setVec3("sky", glm::vec3(red, green, blue));
	shader.setInt("octaves", 3);
}

void SetContinuousUniforms(Shader& terrain, Shader& water) {
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1200.0f);
	glm::mat4 view = camera.GetViewMatrix();

	terrain.use();
	terrain.setMat4("projection", projection);
	terrain.setMat4("view", view);
	terrain.setVec3("viewPos", camera.Position);

	water.use();
	water.setMat4("projection", projection);
	water.setMat4("view", view);
}

void CreateFBO(unsigned int& FBO, unsigned int& colourattatchment, unsigned int& depthattatchment) {
	cout << endl;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	cout << "Created FBO with ID: " << FBO << endl;
	TextureController::CreateFBOCA(colourattatchment, SCR_WIDTH, SCR_HEIGHT);
	TextureController::CreateFBODA(depthattatchment, SCR_WIDTH, SCR_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





