#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <memory>
#include "shader.h"

using std::shared_ptr;
using std::string;

class VisualModel
{
private:
  shared_ptr<Shader> shader;
  unsigned int vbo;
  unsigned int texture;
  unsigned int facesCount;
  bool textured;
public:
  string shaderId;
  string filepath;
  string identifier;
  string texpath;
  VisualModel(
    shared_ptr<Shader> shader,
    string shaderId,
    string filepath,
    string identifier
  );
  VisualModel(
    shared_ptr<Shader> shader,
    string shaderId,
    string objpath,
    string identifier,
    string texpath
  );
  ~VisualModel();
  void draw(float* viewMatrix);
  void bind();
};