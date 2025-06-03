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
    fbWidth(width), fbHeight(height), window(nullptr),
    camera(glm::vec3(0.f, 2.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
    this->InitGLFW();
    this->InitWindow(title, resizable);
    this->InitGLEW();
    this->InitOpenGLOptions();
    
    this->InitShaders();
    this->InitTextures();
    this->InitMaterials();
    this->InitMeshes();

    this->terrain = new Terrain("./textures/Rolling_Hills_Height_Map.png", 14);

    this->InitMatrices();
    this->InitOBJModels();
    this->InitModels();
    this->InitLights();

    this->dt = 0.f;
    this->curTime = 0.f;
    this->lastTime = 0.f;

    this->lastMouseX = 0.0;
    this->lastMouseY = 0.0;
    this->mouseX = 0.0;
    this->mouseY = 0.0;
    this->mouseOffsetX = 0.0;
    this->mouseOffsetY = 0.0;
    this->firstMouse = true;

    //this->InitUniforms();  mozda zatreba sad me mrzi

    this->Init();
}

Game::~Game()
{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    for (auto*& i : this->Models)
    {
        delete i;
    }
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

    // sa 0 je iskljucen vsync
    glfwSwapInterval(1);

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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  GL_TRIANGLES
    // nesto se desi sa teksturama p ane radi bas najbolje - proveri zasto
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::InitMatrices()
{
    this->fov = 90.f;
    this->nearPlane = 0.1f;
    this->farPlane = 1000.f;
    glm::mat4 projection(1.f);
    this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
        static_cast<float>(this->Width) / (this->Height),
        this->nearPlane, this->farPlane);

    ResourceManager::GetShader("sprite")->SetMatrix4("projection", this->ProjectionMatrix, true);
    ResourceManager::GetShader("sprite")->SetMatrix4("view", this->camera.GetViewMatrix());

    ResourceManager::GetShader("terrain")->SetMatrix4("projection", this->ProjectionMatrix, true);
    ResourceManager::GetShader("terrain")->SetMatrix4("view", this->camera.GetViewMatrix());
    ResourceManager::GetShader("terrain")->SetMatrix4("model", glm::mat4(1.f));
}

void Game::InitShaders()
{
    ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/terrain.vert", "shaders/terrain.frag", nullptr, "terrain");
}

void Game::InitTextures()
{
    ResourceManager::LoadTexture("textures/avatar.jpg", false, "avatar");
    ResourceManager::LoadTexture("textures/avatar_specular.jpg", false, "avatar_specular");
    ResourceManager::LoadTexture("textures/container2.png", true, "container");
    ResourceManager::LoadTexture("textures/container2_specular.png", true, "container_specular");
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/awesomeface_specular.png", true, "face_specular");
    ResourceManager::LoadTexture("textures/forest_texture.png", true, "forest");
    ResourceManager::LoadTexture("textures/grass.png", true, "grass");
    ResourceManager::LoadTexture("./textures/Rolling_Hills_Bitmap_1025.png", false, "terrain");
}

void Game::InitMaterials()
{
    unsigned int texure_id = ResourceManager::GetTexture("forest")->ID;
    ResourceManager::LoadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 
        texure_id, texure_id, "material0");
}

void Game::InitMeshes()
{
}

void Game::InitModels()
{
    for (auto& pair : ResourceManager::Meshes) {
        delete pair.second;  // Extracting Mesh* from map
    }
    ResourceManager::Meshes.clear();

    //Pyramid test = Pyramid();
    //ResourceManager::LoadMesh(&test, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f), "test");
}

void Game::InitOBJModels()
{
    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("forest"),
        ResourceManager::GetTexture("forest"),
        "assets/obj_files/Tree_4_C_Color1.obj"));
    this->UpdateModelPosition(this->Models[0], glm::vec3(10.f, 0.f, 0.f));
}

void Game::InitLights()
{
    ResourceManager::GetShader("sprite")->SetVector3f("lightPos0", glm::vec3(0.f, 0.f, 2.f));
    ResourceManager::LoadLight(glm::vec3(0.f, 0.f, 2.f), "main_light");
}

void Game::Init()
{
}

void Game::UpdateUniforms()
{
    // Update view matric (camera)
    this->ViewMatrix = this->camera.GetViewMatrix();
    ResourceManager::GetShader("sprite")->SetMatrix4("view", this->ViewMatrix, true);
    ResourceManager::GetShader("sprite")->SetVector3f("camPosition", this->camera.GetPosition());

    ResourceManager::GetShader("terrain")->SetMatrix4("view", this->ViewMatrix, true);
    ResourceManager::GetShader("terrain")->SetMatrix4("model", glm::mat4(1.f));

    // Projection matrix
    glfwGetFramebufferSize(this->window, &this->fbWidth, &this->fbHeight);

    this->ProjectionMatrix = glm::perspective(glm::radians(this->fov),
        static_cast<float>(this->Width) / (this->Height),
        this->nearPlane, this->farPlane);
    ResourceManager::GetShader("sprite")->SetMatrix4("projection", this->ProjectionMatrix);

    ResourceManager::GetShader("terrain")->SetMatrix4("projection", this->ProjectionMatrix);

    // Lights
    ResourceManager::GetShader("sprite")->SetVector3f("lightPos0", *ResourceManager::GetLight("main_light"));
}

void Game::Update()
{
    //ResourceManager::GetMesh("test")->Rotate(glm::vec3(0.f, 100.f * this->dt, 0.f));
    this->Models[0]->Rotate(glm::vec3(0.f, 100.f * this->dt, 0.f));

    this->UpdateUniforms();

    this->camera.UpdateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::UpdateModelPosition(Model* model, glm::vec3 position)
{
    float terrainY = this->terrain->GetHeightAt(position.x, position.z);
    float toBottom = model->CalculateToBottom();
    position.y = toBottom + terrainY;
    model->UpdatePosition(position);
}

void Game::UpdateDT()
{
    this->curTime = static_cast<float>(glfwGetTime());
    this->dt = this->curTime - this->lastTime;
    this->lastTime = this->curTime;
}

void Game::UpdateMouseEvents()
{
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    if (this->firstMouse)
    {
        this->lastMouseX = this->mouseX;
        this->lastMouseY = this->mouseY;
        this->firstMouse = false;
    }

    this->mouseOffsetX = this->mouseX - lastMouseX;
    this->mouseOffsetY = this->mouseY - lastMouseY;

    this->lastMouseX = this->mouseX;
    this->lastMouseY = this->mouseY;
}

void Game::ProcessInput()
{
    if (this->MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
    {
        *ResourceManager::GetLight("main_light") = this->camera.GetPosition();
        this->MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = false;
    }

    if (this->Keys[GLFW_KEY_A]) {
        float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
        this->camera.Move(this->dt, LEFT, terrainY);
    }
    if (this->Keys[GLFW_KEY_D]) {
        float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
        this->camera.Move(this->dt, RIGHT, terrainY);
    }
    if (this->Keys[GLFW_KEY_W]) {
        float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
        this->camera.Move(this->dt, FORWARD, terrainY);
    }
    if (this->Keys[GLFW_KEY_S]) {
        float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
        this->camera.Move(this->dt, BACK, terrainY);
    }

    this->UpdateMouseEvents();
}

void Game::Render()
{
    this->Models[0]->Render(ResourceManager::GetShader("sprite"));

    ResourceManager::GetShader("terrain")->Use();
    ResourceManager::GetTexture("terrain")->Bind();
    ResourceManager::GetShader("terrain")->SetInteger("terrainTexture", ResourceManager::GetTexture("terrain")->ID);
    this->terrain->Render();
}