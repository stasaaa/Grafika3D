#include "mesh.h"

Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices,
		   GLuint* indexArray, const unsigned& nrOfIndices,
           glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->Position = position;
    this->Rotation = rotation;
    this->Scale = scale;

    this->nrOfVertices = nrOfVertices;
    this->nrOfIndices = nrOfIndices;

    this->vertexArray = new Vertex[this->nrOfVertices];
    for (size_t i = 0; i < nrOfVertices; i++)
    {
        this->vertexArray[i] = vertexArray[i];
    }

    this->indexArray = new GLuint[this->nrOfIndices];
    for (size_t i = 0; i < nrOfIndices; i++)
    {
        this->indexArray[i] = indexArray[i];
    }

	this->InitVAO();
	this->UpdateModelMatrix(glm::vec3(0.f));
}

Mesh::Mesh(const Mesh& obj)
{
    this->Position = obj.Position;
    this->Rotation = obj.Rotation;
    this->Scale = obj.Scale;

    this->nrOfVertices = obj.nrOfVertices;
    this->nrOfIndices = obj.nrOfIndices;

    this->vertexArray = new Vertex[this->nrOfVertices];
    for (size_t i = 0; i < this->nrOfVertices; i++)
    {
        this->vertexArray[i] = obj.vertexArray[i];
    }

    this->indexArray = new GLuint[this->nrOfIndices];
    for (size_t i = 0; i < this->nrOfIndices; i++)
    {
        this->indexArray[i] = obj.indexArray[i];
    }

    this->InitVAO();
    this->UpdateModelMatrix(glm::vec3(0.f));
}

Mesh::Mesh(Primitive* primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->Position = position;
    this->Rotation = rotation;
    this->Scale = scale;

    this->nrOfVertices = primitive->GetNrOfVertices();
    this->nrOfIndices = primitive->GetNrOfIndices();

    this->vertexArray = new Vertex[this->nrOfVertices];
    for (size_t i = 0; i < this->nrOfVertices; i++)
    {
        this->vertexArray[i] = primitive->GetVertices()[i];
    }

    this->indexArray = new GLuint[this->nrOfIndices];
    for (size_t i = 0; i < this->nrOfIndices; i++)
    {
        this->indexArray[i] = primitive->GetIndices()[i];
    }

    this->InitVAO();
    this->UpdateModelMatrix(glm::vec3(0.f));
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);

    delete[] this->vertexArray;
    delete[] this->indexArray;
}

void Mesh::InitVAO()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    if (this->nrOfIndices > 0) 
    {
        glGenBuffers(1, &this->EBO);
    }

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);
    
    if (this->nrOfIndices > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
    }
    
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

void Mesh::UpdateModelMatrix(glm::vec3 origin)
{
    this->ModelMatrix = glm::mat4(1.0f);
    this->ModelMatrix = glm::translate(this->ModelMatrix, origin);  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    //this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(0.5f * this->Scale.x, 0.5f * this->Scale.y, 0.5f * this->Scale.z)); // move origin of rotation to center of quad
    ////                                                                                         x     y     z   - osa po kojoj se okrece
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // then rotate
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    //this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(-0.5f * this->Scale.x, -0.5f * this->Scale.y, -0.5f * this->Scale.z)); // move origin back
    //this->ModelMatrix = glm::translate(this->ModelMatrix, origin -this->Position);
    this->ModelMatrix = glm::translate(this->ModelMatrix, this->Position);
    this->ModelMatrix = glm::scale(this->ModelMatrix, this->Scale); // last scale
}

void Mesh::Update()
{

}

void Mesh::Render(Shader* shader, const glm::vec3& origin)
{
    shader->Use();

    this->UpdateModelMatrix(origin);
    this->UpdateUniforms(shader);

    glBindVertexArray(this->VAO);
    if (this->nrOfIndices == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
    }
    else 
    {
        glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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

void Mesh::SetRotation(const glm::vec3 rotation)
{
    this->Rotation = rotation;
}

void Mesh::ScaleChange(const glm::vec3 scale)
{
    this->Scale += scale;
}

Vertex* Mesh::GetVertices() const
{
    return this->vertexArray;
}

unsigned Mesh::GetVertexCount() const 
{ 
    return this->nrOfVertices; 
}