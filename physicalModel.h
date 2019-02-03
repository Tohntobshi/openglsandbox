#pragma once
#include <memory>
#include "visualModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::shared_ptr;
using glm::vec3;
using glm::mat4;

class PhysicalWorld;

class PhysicalModel {
private:
  shared_ptr<VisualModel> visualModel;
  vec3 position;
  vec3 direction;
  float speed;
  float targetSpeed;
  float width;
  float length;
  float height;
  float pitch;
  float yaw;
  float roll;
  float scale;
  void update();
public:
  PhysicalModel(shared_ptr<VisualModel>, vec3 position);
  void move(vec3 direction);
  void move();
friend PhysicalWorld;
};