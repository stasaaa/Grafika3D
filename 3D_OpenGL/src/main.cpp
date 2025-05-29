#include <iostream>

// SPRITE RENDERER NO LONGER USED
// TODO REMOVE IT

#include "core/game.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* Title = "3D OpenGL";
int fbWidth = 0;
int fbHeight = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game Project(Title, SCREEN_WIDTH, SCREEN_HEIGHT, 3, 3, false);

int main() {
	glfwSetKeyCallback(Project.window, key_callback);

	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(Project.window)) {
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//UPDATE INPUT ---
		glfwPollEvents();

		// manage user input
		// -----------------
		Project.ProcessInput(deltaTime);

		//UPDATE ---
		Project.Update(deltaTime);

		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//DREW
		Project.Render();

		//END DRAW
		glfwSwapBuffers(Project.window);
		glFlush();
	}

	//glfwTerminate();

	return 0;
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