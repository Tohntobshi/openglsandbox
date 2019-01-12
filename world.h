#pragma once

#include "model.h"
#include "shader.h"
#include <list>


class World
{
  private:
  std::list<Model*> objects;
  std::list<Shader*> shaders;
  float verticalRotation;
  float horizontalRotation;
  float cameraX;
  float cameraY;
  float cameraZ;
  float screenWidth;
  float screenHeight;
  void updateViewProj();
  public:
  World();
  ~World();
  void addModel(Model*);
  void addShader(Shader*);
  void draw();
};