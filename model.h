#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "shader.h"

class Occurence {
private:
  // float x;
  // float y;
  // float z;
  // float pitch;
  // float yaw;
  // float roll;
  // float scale;
  glm::mat4 model;
public:
  Occurence(float x, float y, float z, float pitch, float yaw, float roll, float scale);
  const float* getMatrix() const;
};

class Model
{
private:
  Shader* shader;
  void bind();
  unsigned int vbo;
  unsigned int texture;
  std::list<Occurence> occurences;
  unsigned int facesCount;
  bool textured;
public:
  Model(Shader* shader, std::string filepath);
  Model(Shader* shader, std::string objpath, std::string texpath);
  ~Model();
  void addOccurence(float x, float y, float z, float pitch, float yaw, float roll, float scale);
  void clearOccurences();
  void draw();
};