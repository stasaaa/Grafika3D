#ifndef GAME_H
#define GAME_H

#include "libs.h"
#include "OBJLoader.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
private:
    void UpdateUniforms();
    void UpdateModelPosition(Model* model, glm::vec3 position);
public:
    GLFWwindow* window;
    int fbWidth;
    int fbHeight;
    //OpenGL context
    const int GL_VER_MAJOR;
    const int GL_VER_MINOR;

    //Matrices
    glm::mat4 ViewMatrix;
    glm::vec3 camPosition;
    glm::vec3 worldUp;
    glm::vec3 camFront;

    glm::mat4 ProjectionMatrix;
    float fov;
    float nearPlane;
    float farPlane;

    // Delta time
    float dt;
    float curTime;
    float lastTime;

    // Mouse Input
    double lastMouseX;
    double lastMouseY;
    double mouseX;
    double mouseY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    Camera camera;
    Terrain* terrain;

    std::vector<Model*> Models;

    // game state
    GameState               State;
    bool                    Keys[1024], MouseButtons[8];
    unsigned int            Width, Height;
    // constructor/destructor
    Game(const char* title,
        const int width, const int height,
        int GLVerMajor, int GLVerMinor,
        bool resizable);
    virtual ~Game();
    // sa tutorijala
    void InitOBJModels();
    void InitGLFW();
    void InitWindow(const char* title, bool resizable);
    void InitGLEW(); // after vontext creation!!!
    void InitOpenGLOptions();

    void InitMatrices();
    void InitShaders();
    void InitTextures();
    void InitMaterials();
    void InitMeshes();
    void InitModels();
    void InitLights();

    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput();
    void UpdateMouseEvents();
    void UpdateDT();
    void Update();
    void Render();
};

#endif