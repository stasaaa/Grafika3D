#include <iostream>

// SPRITE RENDERER NO LONGER USED
// TODO REMOVE IT

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#include "core/game.h"
#include "core/libs.h"

unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;
int fbWidth = 0;
int fbHeight = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void updateInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game Project(SCREEN_WIDTH, SCREEN_HEIGHT);

GLFWwindow* createWindow(
	const char* title,
	const int width, const int height,
	int& fbWidth, int& fbHeight,
	int GLmajorVer, int GLminorVer,
	bool resizable
);

int main() {
	if (!glfwInit())
	{
		std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
		return 1;
	}

	GLFWwindow* window = createWindow("3D projekat", SCREEN_WIDTH, SCREEN_HEIGHT,
		fbWidth, fbHeight, 3, 3, false);
	if (window == NULL)
	{
		return -1;
	}

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW nije mogao da se ucita! :'(\n";
		glfwTerminate();
		return 3;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// BITNO !!!!
	// kada budem htela da stavim wireframe mode (outline) koristim
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// nesto se desi sa teksturama p ane radi bas najbolje - proveri zasto
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Project.Init();

	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//UPDATE INPUT ---
		glfwPollEvents();
		updateInput(window);

		// manage user input
		// -----------------
		Project.ProcessInput(deltaTime);

		//UPDATE ---
		Project.Update(deltaTime);
		//DRAW ---

		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//DREW
		Project.Render();

		//END DRAW
		//glFlush(); -- Is it important?
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Project.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Project.Keys[key] = false;
	}
}

GLFWwindow* createWindow(
	const char* title, 
	const int width, const int height,
	int& fbWidth, int& fbHeight,
	int GLmajorVer, int GLminorVer,
	bool resizable
)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminorVer);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	GLFWwindow* window = glfwCreateWindow(width, height, "3D OpenGL", NULL, NULL);

	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (window == NULL) {
		std::cout << "Could not create window :(" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	return window;
}