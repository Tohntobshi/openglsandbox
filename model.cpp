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
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}