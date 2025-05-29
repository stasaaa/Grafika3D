#include "mesh.h"

Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVerices,
		   GLuint* indexArray, const unsigned& nrOfIndices,
           glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->Position = position;
    this->Rotation = rotation;
    this->Scale = scale;
	this->InitVAO(vertexArray, nrOfVerices, indexArray, nrOfIndices);
	this->UpdateModelMatrix();
}

Mesh::Mesh(Primitive* primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->Position = position;
    this->Rotation = rotation;
    this->Scale = scale;
    this->InitVAO(primitive);
    this->UpdateModelMatrix();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}


void Mesh::InitVAO(Vertex* vertexArray, const unsigned& nrOfVerices,
                   GLuint* indexArray, const unsigned& nrOfIndices)
{
    this->nrOfVertices = nrOfVerices;
    this->nrOfIndices = nrOfIndices;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Mesh::InitVAO(Primitive* primitive)
{
    this->nrOfVertices = primitive->GetNrOfVertices();
    this->nrOfIndices = primitive->GetNrOfIndices();

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->GetIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLuint*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Mesh::UpdateUniforms(Shader* shader)
{
    shader->SetMatrix4("model", this->ModelMatrix, true);
}

void Mesh::UpdateModelMatrix()
{
    this->ModelMatrix = glm::mat4(1.0f);
    this->ModelMatrix = glm::translate(this->ModelMatrix, this->Position);  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(0.5f * this->Scale.x, 0.5f * this->Scale.y, 0.0f)); // move origin of rotation to center of quad
    ////                                                                                         x     y     z   - osa po kojoj se okrece
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // then rotate
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(-0.5f * this->Scale.x, -0.5f * this->Scale.y, 0.0f)); // move origin back
    this->ModelMatrix = glm::scale(this->ModelMatrix, this->Scale); // last scale
}

void Mesh::Update()
{

}

void Mesh::Render(Shader* shader)
{
    shader->Use();

    this->UpdateModelMatrix();
    this->UpdateUniforms(shader);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::Move(const glm::vec3 position)
{
    this->Position += position;
}

void Mesh::Rotate(const glm::vec3 rotation)
{
    this->Rotation += rotation;
    if (this->Rotation.x > 360)
        this->Rotation.x = 0;
    if (this->Rotation.y > 360)
        this->Rotation.y = 0;
    if (this->Rotation.z > 360)
        this->Rotation.z = 0;
    if (this->Rotation.x < -360)
        this->Rotation.x = 0;
    if (this->Rotation.y < -360)
        this->Rotation.y = 0;
    if (this->Rotation.z < -360)
        this->Rotation.z = 0;
}

void Mesh::ScaleChange(const glm::vec3 scale)
{
    this->Scale += scale;
}