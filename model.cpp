#include "model.h"
#include <SDL2/SDL_opengles2.h>
#include <iostream>

void clearGlErrors();
void checkGLErrors();


Model::Model(Shader* shader)
  : shader(shader)
{

  float vertices[] = {
    -0.5f,  0.5f,// Top-left
    0.5f,  0.5f, // Top-right
    0.5f, -0.5f,// Bottom-right
    -0.5f, -0.5f, // Bottom-left
  };
  unsigned int elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

Model::~Model()
{
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);
}

void Model::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  shader->use();
  glVertexAttribPointer(shader->getAttributeLocation("position"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

void Model::draw()
{
  bind();
  int uniformLocation = shader->getUniformLocation("model");
  for (Occurence& occurence: occurences)
  {
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, occurence.getMatrix());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
}

void Model::addOccurence(float x, float y, float z, float pitch, float yaw, float roll, float scale)
{
  occurences.emplace_back(x, y, z, pitch, yaw, roll, scale);
}

void Model::clearOccurences()
{
  occurences.clear();
}

Occurence::Occurence(float x, float y, float z, float pitch, float yaw, float roll, float scale)
  // : x(x), y(y), z(z), pitch(pitch), yaw(yaw), roll(roll), scale(scale)
{
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(x, y, z));
  model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale));
  // here
}

const float* Occurence::getMatrix() const
{
  return glm::value_ptr(model);
}