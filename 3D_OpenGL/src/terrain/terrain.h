#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <SOIL2/SOIL2.h>
#include <stb/stb_image.h>

template<typename T>
T clamp(T val, T minVal, T maxVal) {
    if (val < minVal) return minVal;
    else if (val > maxVal) return maxVal;
    else return val;
}

class Terrain {
public:
    Terrain() {}
    Terrain(const std::string& heightmapPath,float yOffet = 0.f, 
        float yScale = 64.0f / 256.0f, float yShift = 16.0f, int resolution = 1);
    ~Terrain();
    float GetHeightAt(float x, float z) const;
    void Render();

private:
    void LoadHeightmap(const std::string& path);
    void SetupMesh();

    std::vector<float> vertices;
    std::vector<unsigned> indices;
    std::vector<float> heightValues;
    float horizontalScale;

    int width, height, nChannels;
    unsigned char* heightData;
    unsigned int NUM_STRIPS, NUM_VERTS_PER_STRIP;

    GLuint VAO, VBO, EBO;

    float yScale, yShift, yOffset;
    int resolution;
};

#endif
