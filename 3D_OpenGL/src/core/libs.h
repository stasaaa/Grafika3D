#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resource_manager.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/material.h"
#include "../graphics/vertex.h"
#include "../entities/mesh.h"
#include "../entities/primitives.h"
#include "camera.h"
#include "../entities/model.h"
#include "OBJLoader.h"
#include "light.h"
#include "../terrain/terrain.h"