#include "game.h"

float pom = 0.f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Game::Game(const char* title,
    const int width, const int height,
    int GLVerMajor, int GLVerMinor,
    bool resizable)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height),
    GL_VER_MAJOR(GLVerMajor), GL_VER_MINOR(GLVerMinor),
    fbWidth(width), fbHeight(height), window(nullptr)
{
    this->InitGLFW();
    this->InitWindow(title, resizable);
    this->InitGLEW();
    this->InitOpenGLOptions();
    
    this->InitShaders();
    this->InitTextures();
    this->InitMaterials();
    this->InitMeshes();

    this->InitMatrices();
    this->InitLights();

    //this->InitUniforms();  mozda zatreba sad me mrzi

    this->Init();
}

Game::~Game()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Game::InitGLFW()
{
    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        glfwTerminate();
    }
}

void Game::InitWindow(const char* title, bool resizable)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VER_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VER_MINOR);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    GLFWwindow* window = glfwCreateWindow(this->Width, this->Height, title, NULL, NULL);

    glfwGetFramebufferSize(window, &this->fbWidth, &this->fbHeight);
    glViewport(0, 0, this->Width, this->Height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (window == NULL) {
        std::cout << "Could not create window :(" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    this->window = window;
}

void Game::InitGLEW()
{
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        glfwTerminate();
    }
}

void Game::InitOpenGLOptions()
{
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
}

void Game::InitMatrices()
{
    this->worldUp = glm::vec3(0.f, 1.f, 0.f);
    this->camFront = glm::vec3(0.f, 0.f, -1.f);
    this->camPosition = glm::vec3(0.f, 0.f, 1.f);
    glm::mat4 view(1.f);
    this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

    this->fov = 90.f;
    this->nearPlane = 0.1f;
    this->farPlane = 100.f;
    glm::mat4 projection(1.f);
    this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
        static_cast<float>(this->Width) / (this->Height),
        this->nearPlane, this->farPlane);

    ResourceManager::GetShader("sprite")->Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite")->SetMatrix4("projection", this->ProjectionMatrix);
    ResourceManager::GetShader("sprite")->SetMatrix4("view", this->ViewMatrix);
    ResourceManager::GetShader("sprite")->SetVector3f("camPosition", this->camPosition);
}

void Game::InitShaders()
{
    ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
}

void Game::InitTextures()
{
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
}

void Game::InitMaterials()
{
    ResourceManager::LoadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), ResourceManager::GetTexture("face")->ID, ResourceManager::GetTexture("face")->ID, "material0");
}

void Game::InitMeshes()
{
    Quad quad = Quad();
    ResourceManager::LoadMesh(&quad, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f), "test");
}

void Game::InitLights()
{
    ResourceManager::GetShader("sprite")->SetVector3f("lightPos0", glm::vec3(0.f, 0.f, 2.f));
}

void Game::Init()
{
}

void Game::Update(float dt)
{
    ResourceManager::GetTexture("face")->Bind();

    ResourceManager::GetMaterial("material0")->SendToShader(*ResourceManager::GetShader("sprite"));
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_A]) {
        ResourceManager::GetMesh("test")->Move(glm::vec3(-1.f * dt, 0.0f, 0.0f));
    }
    if (this->Keys[GLFW_KEY_D]) {
        ResourceManager::GetMesh("test")->Move(glm::vec3(1.f * dt, 0.0f, 0.0f));
    }
    if (this->Keys[GLFW_KEY_Q]) {
        ResourceManager::GetMesh("test")->Rotate(glm::vec3(0.f, -100.0f * dt, 0.0f));
    }
    if (this->Keys[GLFW_KEY_E]) {
        ResourceManager::GetMesh("test")->Rotate(glm::vec3(0.f, 100.0f * dt, 0.0f));
    }
    if (this->Keys[GLFW_KEY_Z]) {
        ResourceManager::GetMesh("test")->ScaleChange(glm::vec3(-1.f * dt));
    }
    if (this->Keys[GLFW_KEY_X]) {
        ResourceManager::GetMesh("test")->ScaleChange(glm::vec3(1.f * dt));
    }
}

void Game::Render()
{
    //Texture2D* face = ResourceManager::GetTexture("face");
    /*pom += 0.02f;
    if (pom >= 360) {
        pom = 0.f;
    }*/
    //Renderer->DrawSprite(face, glm::vec3(.0f, 0.f, .0f), glm::vec3(1.0f), glm::vec3(-pom, 0.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f));
    //Shader* basicShader = ResourceManager::GetShader("sprite");
    //Mesh& test = ResourceManager::GetMesh("test");
    ResourceManager::GetMesh("test")->Render(ResourceManager::GetShader("sprite"));
}