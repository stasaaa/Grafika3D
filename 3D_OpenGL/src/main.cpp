#include <iostream>
#include <chrono>
#include <thread>

// SPRITE RENDERER NO LONGER USED
// TODO REMOVE IT

//#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H 

#include "core/game.h"
#include "text/text.h"
#include "text/characters.h"

const int TARGET_FPS = 60;
const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

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


    // freetype
    Characters* TextCharacters = new Characters();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/Jersey_10/Jersey10-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned int c = 0; c < 2048; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            TextCharacters->AllCharacters.insert(std::pair<unsigned int, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Project.InitTextRenderer(*TextCharacters);

	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(Project.window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		if (deltaTime < TARGET_FRAME_TIME) {
			//std::cout << "Delta time: " << deltaTime << "\tSleep for: " << static_cast<int>((TARGET_FRAME_TIME - deltaTime) * 1000) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((TARGET_FRAME_TIME - deltaTime) * 1000)));
		}

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