#include "game.h"

TextRenderer* RenderText;
std::wstring NameOutput(L"Staša Radojičić RA 62/2021");

enum MODELS
{
    TREE = 0,
    SUN,
    MOON,
    HOUSE,
    STATUE1,
    STATUE2,
    STATUE3,
    STATUE4
};

std::vector<glm::vec3> rotations = { {glm::vec3(0.f)} ,
    {glm::vec3(0.f)} ,{glm::vec3(0.f)} ,{glm::vec3(0.f)}};
std::vector<float> rotation_speed = { 2.3f, -10.f, -17.f, 5.f};

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
    camera(glm::vec3(8.f, 3.f, 8.f), glm::vec3(1.f, -0.5f, -1.f), glm::vec3(0.f, 1.f, 0.f))
{
    this->InitGLFW();
    this->InitWindow(title, resizable);
    this->InitGLEW();
    this->InitOpenGLOptions();
    
    this->InitShaders();
    this->InitTextures();
    this->InitMaterials();
    this->InitMeshes();

    this->terrain = new Terrain("./textures/Rolling_Hills_Height_Map.png", 14.f);

    this->InitMatrices();
    this->InitOBJModels();
    this->InitModels();
    this->InitLights();
    this->InitCameras();

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(Width), 0.0f, static_cast<float>(Height));
    ResourceManager::GetShader("text")->SetMatrix4("projection", projectionText, true);
    ResourceManager::GetShader("text")->SetFloat("textAlpha", 1.f);
}

void Game::InitShaders()
{
    ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/terrain.vert", "shaders/terrain.frag", nullptr, "terrain");
    ResourceManager::LoadShader("shaders/text.vert", "shaders/text.frag", nullptr, "text");
}

void Game::InitTextures()
{
    //ResourceManager::LoadTexture("textures/avatar.jpg", false, "avatar");
    //ResourceManager::LoadTexture("textures/avatar_specular.jpg", false, "avatar_specular");
    //ResourceManager::LoadTexture("textures/container2.png", true, "container");
    //ResourceManager::LoadTexture("textures/container2_specular.png", true, "container_specular");
    //ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    //ResourceManager::LoadTexture("textures/awesomeface_specular.png", true, "face_specular");
    //ResourceManager::LoadTexture("textures/forest_texture.png", true, "forest");
    //ResourceManager::LoadTexture("textures/grass.png", true, "grass");
    ResourceManager::LoadTexture("./textures/Rolling_Hills_Bitmap_1025.png", false, "terrain");
    ResourceManager::LoadTexture("textures/tiny_treats_texture_1.png", true, "house");
    ResourceManager::LoadTexture("textures/grunge-wall-texture.jpg", false, "stone");
    ResourceManager::LoadTexture("textures/marble-tile-texture.jpg", false, "marble");
}

void Game::InitMaterials()
{
    unsigned int stoneID = ResourceManager::GetTexture("stone")->ID;
    unsigned int houseID = ResourceManager::GetTexture("house")->ID;
    ResourceManager::LoadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 
        stoneID, stoneID, "material0");
    ResourceManager::LoadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
        houseID, houseID, "material1");
}

void Game::InitMeshes()
{
    Pyramid pyramid = Pyramid();
    Quad quad = Quad();
    Cube cube = Cube();
    InvertedPyramid invertedPyramid = InvertedPyramid();
    ResourceManager::LoadMesh(&pyramid, glm::vec3(0.f, 3.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f), "pyramid");
    ResourceManager::LoadMesh(&quad, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f), "quad");
    ResourceManager::LoadMesh(&cube, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), "cube");
    ResourceManager::LoadMesh(&invertedPyramid, glm::vec3(0.f, 2.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), "invertedPyramid");
}

void Game::InitModels()
{
    std::vector<Mesh*> meshes;
    meshes.push_back(ResourceManager::GetMesh("pyramid"));
    meshes.push_back(ResourceManager::GetMesh("cube"));
    meshes.push_back(ResourceManager::GetMesh("invertedPyramid"));

    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("stone"),
        ResourceManager::GetTexture("stone"),
        meshes));

    meshes.clear();

    ResourceManager::GetMesh("cube")->ScaleChange(glm::vec3(2.f, 1.f, 2.f));
    ResourceManager::GetMesh("invertedPyramid")->ScaleChange(glm::vec3(2.f, 1.f, 2.f));

    meshes.push_back(ResourceManager::GetMesh("cube"));
    meshes.push_back(ResourceManager::GetMesh("pyramid"));

    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("marble"),
        ResourceManager::GetTexture("marble"),
        meshes));

    meshes.clear();

    ResourceManager::GetMesh("cube")->ScaleChange(glm::vec3(-2.f, -1.f, -2.f));
    ResourceManager::GetMesh("cube")->ScaleChange(glm::vec3(0.8f, 1.2f, 1.2f));
    ResourceManager::GetMesh("pyramid")->ScaleChange(glm::vec3(0.8f, 1.2f, 1.2f));

    meshes.push_back(ResourceManager::GetMesh("cube"));
    meshes.push_back(ResourceManager::GetMesh("pyramid"));

    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("stone"),
        ResourceManager::GetTexture("stone"),
        meshes));

    meshes.clear();
    
    ResourceManager::GetMesh("pyramid")->Move(glm::vec3(0.f, -1.f, 0.f));
    meshes.push_back(ResourceManager::GetMesh("pyramid"));
    ResourceManager::GetMesh("cube")->ScaleChange(glm::vec3(-0.8f, -1.2f, -1.2f));
    meshes.push_back(ResourceManager::GetMesh("cube"));

    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("marble"),
        ResourceManager::GetTexture("marble"),
        meshes));

    meshes.clear();

    //this->UpdateModelPosition(this->Models[4], glm::vec3(2.f, 0.f, 0.f));


    for (auto& pair : ResourceManager::Meshes) {
        delete pair.second;  // Extracting Mesh* from map
    }
    ResourceManager::Meshes.clear();
}

void Game::InitOBJModels()
{
    this->Models.push_back(new Model(glm::vec3(0.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("forest"),
        ResourceManager::GetTexture("forest"),
        "assets/obj_files/Tree_4_C_Color1.obj"));

    this->Models.push_back(new Model(glm::vec3(50.f, 50.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("forest"),
        ResourceManager::GetTexture("forest"),
        "assets/obj_files/Sun_483.obj"));

    this->Models.push_back(new Model(glm::vec3(-50.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material0"),
        ResourceManager::GetTexture("forest"),
        ResourceManager::GetTexture("forest"),
        "assets/obj_files/Moon.obj"));

    this->Models.push_back(new Model(glm::vec3(-50.f, 0.f, 0.f),
        ResourceManager::GetMaterial("material1"),
        ResourceManager::GetTexture("house"),
        ResourceManager::GetTexture("house"),
        "assets/obj_files/house.obj"));
    this->Models[3]->UpdateScale(glm::vec3(1.5f));
    this->Models[3]->Rotate(glm::vec3(0.f, 90.f, 0.f));

    this->UpdateModelPosition(this->Models[TREE], glm::vec3(10.f, 0.f, 0.f));
    this->UpdateModelPosition(this->Models[HOUSE], glm::vec3(-30.f, 0.f, 0.f));
}

void Game::InitLights()
{
    ResourceManager::GetShader("sprite")->SetVector3f("lightPos0", glm::vec3(0.f, 1000.f, 0.f));
    ResourceManager::LoadLight(glm::vec3(0.f, 100.f, 0.f), "main_light");
}

void Game::InitCameras()
{
    float terrainY = this->terrain->GetHeightAt(8.f, 8.f);
    this->camera.CameraChanged(glm::vec3(8.f, 3.f + terrainY, 8.f), glm::vec3(1.f, -0.5f, -1.f));
}

void Game::InitTextRenderer(Characters& textCharacters)
{
    RenderText = new TextRenderer(*ResourceManager::GetShader("text"), textCharacters);
}

void Game::Init()
{
    this->camera.Move(this->dt, FORWARD, this->terrain->GetHeightAt(0.f, 0.f));
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
    for (unsigned int i = 0; i < 4; i++) {
        rotations[i] += glm::vec3(0.f, rotation_speed[i] * this->dt, 0.f);
    }

    //ResourceManager::GetMesh("test")->Rotate(glm::vec3(0.f, 100.f * this->dt, 0.f));
    this->Models[0]->Rotate(glm::vec3(0.f, 100.f * this->dt, 0.f));

    this->UpdateUniforms();

    //this->camera.UpdateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);
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

    //if (this->Keys[GLFW_KEY_A]) {
    //    float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
    //    this->camera.Move(this->dt, LEFT, terrainY);
    //}
    //if (this->Keys[GLFW_KEY_D]) {
    //    float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
    //    this->camera.Move(this->dt, RIGHT, terrainY);
    //}
    //if (this->Keys[GLFW_KEY_W]) {
    //    float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
    //    this->camera.Move(this->dt, FORWARD, terrainY);
    //}
    //if (this->Keys[GLFW_KEY_S]) {
    //    float terrainY = this->terrain->GetHeightAt(this->camera.GetPosition().x, this->camera.GetPosition().z);
    //    this->camera.Move(this->dt, BACK, terrainY);
    //}
    if (this->Keys[GLFW_KEY_1]) {
        float terrainY = this->terrain->GetHeightAt(8.f, 8.f);
        this->camera.CameraChanged(glm::vec3(8.f, 3.f + terrainY, 8.f), glm::vec3(1.f, -0.5f, -1.f));
    }
    if (this->Keys[GLFW_KEY_2]) {
        float terrainY = this->terrain->GetHeightAt(8.f, -8.f);
        this->camera.CameraChanged(glm::vec3(8.f, 3.f + terrainY, -8.f), glm::vec3(-1.f, -0.5f, -1.f));
    }
    if (this->Keys[GLFW_KEY_3]) {
        float terrainY = this->terrain->GetHeightAt(-8.f, -8.f);
        this->camera.CameraChanged(glm::vec3(-8.f, 3.f + terrainY, -8.f), glm::vec3(-1.f, -0.5f, 1.f));
    }
    if (this->Keys[GLFW_KEY_4]) {
        float terrainY = this->terrain->GetHeightAt(-8.f, -8.f);
        this->camera.CameraChanged(glm::vec3(-8.f, 3.f + terrainY, 8.f), glm::vec3(1.f, -0.5f, 1.f));
    }

    this->UpdateMouseEvents();
}

void Game::Render()
{
    this->Models[HOUSE]->SetRotation(glm::vec3(0.f, 90.f, 0.f));
    this->UpdateModelPosition(this->Models[HOUSE], glm::vec3(-30.f, 0.f, 0.f));
    this->Models[HOUSE]->Render(ResourceManager::GetShader("sprite"));

    this->Models[HOUSE]->SetRotation(glm::vec3(0.f, -110.f, 0.f));
    this->UpdateModelPosition(this->Models[HOUSE], glm::vec3(30.f, 0.f, 50.f));
    this->Models[HOUSE]->Render(ResourceManager::GetShader("sprite"));

    this->Models[STATUE1]->SetRotation(rotations[0]);
    this->UpdateModelPosition(this->Models[STATUE1], glm::vec3(5.f, 0.f, 5.f));
    this->Models[STATUE1]->Render(ResourceManager::GetShader("sprite"));

    this->Models[STATUE2]->SetRotation(rotations[1]);
    this->UpdateModelPosition(this->Models[STATUE2], glm::vec3(-5.f, 0.f, 5.f));
    this->Models[STATUE2]->Render(ResourceManager::GetShader("sprite"));

    this->Models[STATUE3]->SetRotation(rotations[2]);
    this->UpdateModelPosition(this->Models[STATUE3], glm::vec3(-5.f, 0.f, -5.f));
    this->Models[STATUE3]->Render(ResourceManager::GetShader("sprite"));

    this->Models[STATUE4]->SetRotation(rotations[3]);
    this->UpdateModelPosition(this->Models[STATUE4], glm::vec3(5.f, 0.f, -5.f));
    this->Models[STATUE4]->Render(ResourceManager::GetShader("sprite"));

    ResourceManager::GetShader("terrain")->Use();
    ResourceManager::GetTexture("terrain")->Bind();
    ResourceManager::GetShader("terrain")->SetInteger("terrainTexture", 0);
    this->terrain->Render();

    RenderText->RenderText(NameOutput, 25.0f, 45.0f, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f));
}