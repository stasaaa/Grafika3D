#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../graphics/texture.h"
#include "../graphics/sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec3   Color, Rotation, Position, Size, Velocity;
    float       Alpha;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, float alpha = 1.0f
        ,glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
    bool IsClicked(float mouseX, float mouseY) const;
};

#endif