#include "game.h"

#include "resource_manager.h"
#include "../graphics/sprite_renderer.h"
#include "../entities/mesh.h"
#include "primitives.h"

SpriteRenderer* Renderer;
Mesh* test = nullptr;

float pom = 0.f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{

    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    /*glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(this->Width),
        0.0f, static_cast<float>(this->Height),
        -1.0f, 1.0f
    );*/

    float fov = 90.f;
    float nearPlane = 0.1f;
    float farPlane = 100.f;
    glm::mat4 projection(1.f);
    projection = glm::perspective(glm::radians(fov),
        static_cast<float>(Width) / (Height), 
        nearPlane, farPlane);

    glm::vec3 worldUp(0.f, 1.f, 0.f);
    glm::vec3 camFront(0.f, 0.f, -1.f);
    glm::vec3 camPosition(0.f, 0.f, 1.f);
    glm::mat4 view(1.f);
    view = glm::lookAt(camPosition, camPosition + camFront, worldUp);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);

    glm::vec3 lightPos0(0.f, 0.f, 2.f);

    ResourceManager::GetShader("sprite").SetVector3f("lightPos0", lightPos0);
    ResourceManager::GetShader("sprite").SetVector3f("camPosition", camPosition);

    // set render-specific controls
    Shader basicShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(basicShader);
    // load textures
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");

    Texture2D face = ResourceManager::GetTexture("face");
    ResourceManager::LoadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), face.ID, face.ID, "material0");

    // TEST MESH
    Quad quad = Quad();
    test = new Mesh(&quad, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f));
}

void Game::Update(float dt)
{
    ResourceManager::GetTexture("face").Bind();

    Shader basicShader = ResourceManager::GetShader("sprite");
    ResourceManager::GetMaterial("material0").SendToShader(basicShader);
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_A]) {
        test->Move(glm::vec3(-1.f *dt , 0.0f, 0.0f));
    }
    if (this->Keys[GLFW_KEY_D]) {
        test->Move(glm::vec3(1.f * dt, 0.0f, 0.0f));
    }
    if (this->Keys[GLFW_KEY_Q]) {
        test->Rotate(glm::vec3(0.f, -100.0f * dt, 0.0f));
    }
    if (this->Keys[GLFW_KEY_E]) {
        test->Rotate(glm::vec3(0.f, 100.0f * dt, 0.0f));
    }
    if (this->Keys[GLFW_KEY_Z]) {
        test->ScaleChange(glm::vec3(-1.f * dt));
    }
    if (this->Keys[GLFW_KEY_X]) {
        test->ScaleChange(glm::vec3(1.f * dt));
    }
}

void Game::Render()
{
    Texture2D face = ResourceManager::GetTexture("face");
    pom += 0.02f;
    if (pom >= 360) {
        pom = 0.f;
    }
    //Renderer->DrawSprite(face, glm::vec3(.0f, 0.f, .0f), glm::vec3(1.0f), glm::vec3(-pom, 0.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f));
    Shader basicShader = ResourceManager::GetShader("sprite");
    test->Render(&basicShader);
}