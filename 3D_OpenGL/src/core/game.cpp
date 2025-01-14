#include "game.h"

#include "resource_manager.h"
#include "../graphics/sprite_renderer.h"

SpriteRenderer* Renderer;

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
    // set render-specific controls
    Shader basicShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(basicShader);
    // load textures
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{
    Texture2D face = ResourceManager::GetTexture("face");
    pom += 2.f;
    if (pom >= 360) {
        pom = 0.f;
    }
    Renderer->DrawSprite(face, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), glm::vec3(0.f, -pom, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
}