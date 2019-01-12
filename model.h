#pragma once
#include <glm/glm.hpp>
#include <list>
#include "shader.h"

struct occurence {
  float x;
  float y;
  float z;
  glm::mat4 model;
};

class Model
{
  private:
  Shader* shader;
  public:
  std::list<occurence> occurences;
  Model(Shader* shader);
  ~Model();
  unsigned int vbo;
  unsigned int ebo;
  void bind();
  void draw();
};