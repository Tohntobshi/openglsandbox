#pragma once

#include "model.h"
#include "shader.h"
#include <list>

enum CameraMode {
  THIRD_PERSON,
  FIRST_PERSON,
};

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
  CameraMode cameraMode;
  public:
  World();
  ~World();
  void moveStraight(float amount);
  void moveSideways(float amount);
  void moveXTo(float x);
  void moveYTo(float y);
  void moveZTo(float z);
  void rotateVertTo(float deg);
  void rotateHorTo(float deg);
  void rotateBy(float hor, float vert);
  void moveBy(float x, float y, float z);
  void changeCameraMode(CameraMode mode);
  void updateViewProj();
  void addModel(Model*);
  void addShader(Shader*);
  void draw();
};