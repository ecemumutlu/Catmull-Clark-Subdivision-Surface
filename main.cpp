#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "./sceneObjectRenderers/Object.h"
#include "./sceneObjectRenderers/Structs.h"
#include "./sceneObjectRenderers/CatmullClark.h"
#include "./sceneObjectRenderers/Face.h"
#include "sceneObjectRenderers/Cube.h"
#include "sceneObjectRenderers/Octahedron.h"
#include "sceneObjectRenderers/Tetrahedron.h"
#include <chrono>

#include <iostream>
#include <map>

#define CUBE_RED (glm::vec3(1.0f, 0.0f, 0.0f))
#define OCTAHEDRON_YELLOW (glm::vec3(1.0f, 1.0f, 0.0f))
#define TETRAHEDRON_BLUE (glm::vec3(1.0f, 0.0f, 1.0f))

using namespace std;

int gWidth, gHeight;
static int mode = 0;

glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;

glm::vec3 camera;
glm::vec3 up;
glm::vec3 center;
glm::vec3 eyePos(0, 0, 0);

std::vector<CatmullClark*> gCatmullClarkObjects;
std::vector<Object*> gObjects;

void init()
{
	Cube* cubeObject = new Cube(glm::vec3(0.0f, 0.0f, -15.0f), "./sceneObjects/cube.obj", CUBE_RED, "./shaders/vert.glsl", "./shaders/frag.glsl", glm::vec3(0.0f, 1.0f, 0.0f));
	Octahedron* octahedronObject = new Octahedron(glm::vec3(0.0f, 0.0f, -15.0f), "./sceneObjects/octahedron.obj", OCTAHEDRON_YELLOW, "./shaders/vert.glsl", "./shaders/frag.glsl", glm::vec3(1.0f, 0.0f, 0.0f));	
	Tetrahedron* tetrahedronObject = new Tetrahedron(glm::vec3(0.0f, 0.0f, -15.0f), "./sceneObjects/tetrahedron.obj", TETRAHEDRON_BLUE, "./shaders/vert.glsl", "./shaders/frag.glsl", glm::vec3(1.0f, 0.0f, 0.0f));	

	gObjects.push_back(cubeObject); 
	gObjects.push_back(octahedronObject);
	gObjects.push_back(tetrahedronObject);
	
	CatmullClark* cube = new CatmullClark(*cubeObject, 0);	
	CatmullClark* octahedron = new CatmullClark(*octahedronObject, 1);	// second parameter is the base level
	CatmullClark* tetrahedron = new CatmullClark(*tetrahedronObject, 0);	

	cubeObject = nullptr;
	octahedronObject = nullptr;
	tetrahedronObject = nullptr;

	glEnable(GL_DEPTH_TEST);

    gCatmullClarkObjects.push_back(cube);
	gCatmullClarkObjects.push_back(octahedron);
	gCatmullClarkObjects.push_back(tetrahedron);

	gWidth = 1000;
	gHeight = 800;
}

void drawModel()
{	
	for (auto& obj : gCatmullClarkObjects) {
		obj->drawObject();
	}
	
    assert(glGetError() == GL_NO_ERROR);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind
}


bool areFloatsEqual(float a, float b, float epsilon = 1e-4) {
	return std::abs(a - b) < epsilon;
}

void display(double deltaTime)
{
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (auto& obj : gCatmullClarkObjects) {
		obj->setMatrices(deltaTime);
	}

	if (mode % 3 == 1) {
		for (auto& obj: gCatmullClarkObjects) {
			obj->setDepthBuffer();
		}
	}
	// Draw the scene
	drawModel();
}

void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);

	// Use perspective projection
	float fovyRad = (float)(90.0 / 180.0) * M_PI;
	projectionMatrix = glm::perspective(fovyRad, w / (float)h, 4.0f, 2000.0f);

	// Assume default camera position and orientation (camera is at
	// (0, 0, 0) with looking at -z direction and its up vector pointing
	// at +y direction)
	// 
	//viewingMatrix = glm::mat4(1);
	camera = glm::vec3(0, 0, 0);
	center = glm::vec3(0, 0, 0) + glm::vec3(0, 0, -1000);
	up = glm::vec3(0, 1, 0);
	
	viewingMatrix = glm::lookAt(camera, center, up);
	//viewingMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	for (auto& obj : gCatmullClarkObjects) {
		obj->setProjectionMatrix(projectionMatrix);
		//obj->setViewingMatrix(viewingMatrix);
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) // increase the level
	{
		for(auto& obj : gCatmullClarkObjects) {
			obj->increaseLevel();
		}
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) // decrease the level
	{
		for(auto& obj : gCatmullClarkObjects) {
			obj->decreaseLevel();
		}
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS) // solid -> line -> wire -> solid
	{
		for (auto& obj : gCatmullClarkObjects) {
			obj->changeMode();
		}

		mode = (mode + 1) % 3;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) // reset the whole scene
	{
		for(auto& obj : gCatmullClarkObjects) {
			obj->reset();
			mode = 0;
		}
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) // start/pause animation
	{
		for(auto& obj : gCatmullClarkObjects) {
			obj->start_pause();
		}
	}
}

void mainLoop(GLFWwindow* window)
{
	double lastTime = glfwGetTime();
	int frames = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime(); 
		double deltaTime = currentTime - lastTime; //the result will be in seconds
		lastTime = currentTime;
		display(deltaTime);
		frames++;
		glfwSwapBuffers(window);
		glfwPollEvents();

		auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

        // Update FPS every second
        if (elapsed_time >= 1) {
            double fps = frames / static_cast<double>(elapsed_time);
            std::cout << "FPS: " << fps << std::endl;

            // Reset variables for next second
            frames = 0;
            start_time = end_time;
        }
	}
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		exit(-1);
	}

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this if on MacOS

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); //todo: erase this later, used for error

	int width = 1000, height = 800;
	window = glfwCreateWindow(width, height, "Simple Example", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	char rendererInfo[512] = { 0 };
	strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER)); // Use strcpy_s on Windows, strcpy on Linux
	strcat(rendererInfo, " - "); // Use strcpy_s on Windows, strcpy on Linux
	strcat(rendererInfo, (const char*)glGetString(GL_VERSION)); // Use strcpy_s on Windows, strcpy on Linux
	glfwSetWindowTitle(window, rendererInfo);

	init();
	
	glfwSetKeyCallback(window, keyboard);
	glfwSetWindowSizeCallback(window, reshape);

	reshape(window, width, height); // need to call this once ourselves
	
	mainLoop(window); // this does not return unless the window is closed

	for (auto& obj : gCatmullClarkObjects) {
		delete obj;
	}

	for (auto& obj : gObjects) {
		obj = nullptr;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
