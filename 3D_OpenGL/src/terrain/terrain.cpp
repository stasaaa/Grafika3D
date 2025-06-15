#include "terrain.h"
#include <iostream>

Terrain::Terrain(const std::string& heightmapPath, float yOffset,
    float yScale, float yShift, int resolution)
    : yScale(yScale), yShift(yShift), yOffset(yOffset),resolution(resolution),
    heightData(nullptr), width(0), height(0), nChannels(0),
    VAO(0), VBO(0), EBO(0) {
    LoadHeightmap(heightmapPath);
    SetupMesh();
}

Terrain::Terrain(const std::string& path)
{
    this->LoadOBJFile(path);
    this->SetupMeshOBJ();
}

Terrain::~Terrain() {
    if (heightData) SOIL_free_image_data(heightData);
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Terrain::LoadOBJFile(const std::string& path)
{
    std::vector<Vertex> objVertices = LoadOBJ(path.c_str());
    this->vertices.clear(); // clear previous vertex data
    this->indices.clear();

    this->heightValues.resize(objVertices.size());
    for (size_t i = 0; i < objVertices.size(); ++i)
    {
        const Vertex& v = objVertices[i];

        // store height value y
        this->heightValues[i] = v.position.y;

        // store vertex attributes
        this->vertices.push_back(v.position.x);
        this->vertices.push_back(v.position.y);
        this->vertices.push_back(v.position.z);

        this->vertices.push_back(v.texcoord.x);
        this->vertices.push_back(v.texcoord.y);
    }

    // Now set up OpenGL buffers
    this->SetupMesh();
}

void Terrain::LoadHeightmap(const std::string& path) {
    unsigned char* data = stbi_load(path.c_str(),
        &this->width, &this->height, &this->nChannels,
        0);

    this->heightValues.resize(width * height);

    this->horizontalScale = 0.2f;
    this->yScale = 64.0f / 256.0f;
    for (unsigned int i = 0; i < this->height; i++)
    {
        for (unsigned int j = 0; j < this->width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + this->width * i) * this->nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            float heightValue = ((int)y * this->yScale - this->yShift + this->yOffset);
            this->heightValues[i * width + j] = heightValue;

            // vertex
            this->vertices.push_back((- this->height / 2.0f + i) * this->horizontalScale);        // v.x
            this->vertices.push_back(heightValue); // v.y
            this->vertices.push_back((-this->width / 2.0f + j) * this->horizontalScale);        // v.z
        
            // UV coordinates range from 0.0 to 1.0 across the terrain
            float u = (float)j / (this->width - 1);   // Horizontal progress
            float v = (float)i / (this->height - 1);  // Vertical progress

            this->vertices.push_back(u); // Texture coordinate u
            this->vertices.push_back(v); // Texture coordinate v
        }
    }

    stbi_image_free(data);
}

void Terrain::SetupMeshOBJ()
{
    this->indices.clear();

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
        this->vertices.size() * sizeof(float),       // size of vertices buffer
        &this->vertices[0],                          // pointer to first element
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coords attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Terrain::SetupMesh() {
    for (unsigned int i = 0; i < this->height - 1; i++) {
        for (unsigned int j = 0; j < this->width; j++) {
            this->indices.push_back(j + this->width * (i + 1));  // bottom vertex
            this->indices.push_back(j + this->width * i);        // top vertex
        }
    }

    this->NUM_STRIPS = this->height - 1;
    this->NUM_VERTS_PER_STRIP = this->width * 2;

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,
        this->vertices.size() * sizeof(float),       // size of vertices buffer
        &this->vertices[0],                          // pointer to first element
        GL_STATIC_DRAW);

    // position attribute
    // position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coords attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        this->indices.size() * sizeof(unsigned int), // size of indices buffer
        &this->indices[0],                           // pointer to first element
        GL_STATIC_DRAW);
}

void Terrain::Render() {
    // draw mesh
    glBindVertexArray(this->VAO);

    if (this->indices.size() == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 5.f);
        return;
    }

    // render the mesh triangle strip by triangle strip - each row at a time
    for (unsigned int strip = 0; strip < this->NUM_STRIPS; ++strip)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
            this->NUM_VERTS_PER_STRIP, // number of indices to render
            GL_UNSIGNED_INT,     // index data type
            (void*)(sizeof(unsigned int)
                * this->NUM_VERTS_PER_STRIP
                * strip)); // offset to starting index
    }
}

float Terrain::GetHeightAt(float x, float z) const
{
    if (this->indices.size() == 0)
    {
        float ret = this->GetHeightAtOBJ(x, z);
        return ret;
    }

    // Convert world x,z to heightmap indices i,j
    float i_f = (x / this->horizontalScale) + this->height / 2.0f;
    float j_f = (z / this->horizontalScale) + this->width / 2.0f;

    int i0 = (int)floor(i_f);
    int j0 = (int)floor(j_f);
    int i1 = i0 + 1;
    int j1 = j0 + 1;

    // Clamp indices
    i0 = clamp(i0, 0, (int)height - 1);
    j0 = clamp(j0, 0, (int)width - 1);
    i1 = clamp(i1, 0, (int)height - 1);
    j1 = clamp(j1, 0, (int)width - 1);

    float s = i_f - i0;
    float t = j_f - j0;

    float h00 = heightValues[i0 * width + j0];
    float h10 = heightValues[i1 * width + j0];
    float h01 = heightValues[i0 * width + j1];
    float h11 = heightValues[i1 * width + j1];

    float h0 = h00 * (1 - s) + h10 * s;
    float h1 = h01 * (1 - s) + h11 * s;

    return (h0 * (1 - t) + h1 * t);
}

float Terrain::GetHeightAtOBJ(float x, float z) const
{
    if (heightValues.empty() || vertices.empty())
        return 0.0f;

    float minDistSquared = std::numeric_limits<float>::max();
    float height = 0.0f;

    // vertices stored as x,y,z,u,v flat array: stride 5
    const size_t stride = 5;

    for (size_t i = 0; i < heightValues.size(); ++i)
    {
        float vx = this->vertices[i * stride + 0];
        float vz = this->vertices[i * stride + 2];

        float dx = x - vx;
        float dz = z - vz;
        float distSquared = dx * dx + dz * dz;

        if (distSquared < minDistSquared)
        {
            minDistSquared = distSquared;
            height = this->heightValues[i];
        }
    }

    return height;
}