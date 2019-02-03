#pragma once

#include "shader.h"
#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

enum CameraMode {
  THIRD_PERSON,
  FIRST_PERSON,
};

class Camera
{
private:
  std::list<shared_ptr<Shader>> shaders;
  float verticalRotation;
  float horizontalRotation;
  float cameraX;
  float cameraY;
  float cameraZ;
  CameraMode cameraMode;
public:
  Camera();
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
  void addShader(shared_ptr<Shader> shader);
};