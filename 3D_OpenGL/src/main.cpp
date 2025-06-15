#include <iostream>

// SPRITE RENDERER NO LONGER USED
// TODO REMOVE IT

//#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "core/game.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const char* Title = "3D OpenGL";
int fbWidth = 0;
int fbHeight = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Game Project(Title, SCREEN_WIDTH, SCREEN_HEIGHT, 3, 3, false);

int main() {
	glfwSetKeyCallback(Project.window, key_callback);
	glfwSetMouseButtonCallback(Project.window, mouse_button_callback);

	// deltaTime variables
	// -------------------
	//float deltaTime = 0.0f;
	//float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(Project.window)) {
		// calculate delta time
		// --------------------
		Project.UpdateDT();

		//UPDATE INPUT ---
		glfwPollEvents();

		// manage user input
		// -----------------
		Project.ProcessInput();

		//UPDATE ---
		Project.Update();

		//CLEAR
		glClearColor(0.145f, 0.588f, 0.745f, 1.0f);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		Project.MouseButtons[button] = true;
}