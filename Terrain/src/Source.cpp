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


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

//FrameBuffers
unsigned int MountainFBO, WaterFBO;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void SetTerrainUniforms(Shader& shader);
void SetContinuousUniforms(Shader& terrain, Shader& water);

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
	unsigned int noisetexture = TextureController::CreateTexture(512, 512);
	unsigned int normalmap = TextureController::CreateTexture(512, 512);

	//Noise Generation
	Shader computenoise("..\\Shaders\\ComputeNoise.cms");
	computenoise.use();
	glBindImageTexture(0, noisetexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((GLuint)32, (GLuint)16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//Normal Generation
	Shader computecdm("..\\Shaders\\ComputeCDM.cms");
	computecdm.use();
	computecdm.setInt("scale", 100);
	TextureController::AssignTexture(noisetexture, 0, computecdm, "noisemap");
	glBindImageTexture(0, normalmap, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((GLuint)32, (GLuint)16, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//Terrain Constructor ; number of grids in width, number of grids in height, gridSize
	Terrain terrain(50, 50,10);
	terrain.AssignTerrainTextures("..\\Resources\\stone.png", "..\\Resources\\grass.jpg", "..\\Resources\\sand.png");
	TextureController::AssignTexture(noisetexture, 3, terrain.shader, "noise");
	terrainVAO = terrain.getVAO();

	//Clipping
	glEnable(GL_CLIP_DISTANCE0);
	glm::vec4 plane = glm::vec4(0, 1, 0, -75);
	terrain.shader.setVec4("clipplane", plane);

	//Quad for 2D
	Quad quad;
	quad.CreateQuad();

	//Water Plane
	Water water(75);
	water.CreatePlane();



	SetTerrainUniforms(terrain.shader);

	//FBO Attatchments (BLACK SCREEN IF EITHER OF THESE ARE IN USE)
	//unsigned int mountainCA = TextureController::CreateFBOCA(MountainFBO, SCR_WIDTH, SCR_HEIGHT, 1);
	//unsigned int waterCA = TextureController::CreateFBOCA(WaterFBO, SCR_WIDTH, SCR_HEIGHT, 2);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Bind FBO and draw scene
		//glBindFramebuffer(GL_FRAMEBUFFER, MountainFBO);
		//glEnable(GL_DEPTH_TEST);

		//Clear FBO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render Scene to fill FBO
		SetContinuousUniforms(terrain.shader, water.shader);
		terrain.RenderTerrain();

		//(WATER PLANE DOESN'T RENDER)
		water.RenderPlane();

		//Now use default FBO
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);

		//Clear all buffers
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render to screen
		//quad.RenderQuad(mountainCA);

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
	shader.setVec3("dirlight.direction", glm::vec3(.12f, -.12f, .2f));
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





