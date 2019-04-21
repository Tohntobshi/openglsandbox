#pragma once
#include <memory>
#include "visualModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using std::shared_ptr;

class App;
class Controls;

class PhysicalModel
{
private:
  shared_ptr<VisualModel> visualModel;
public:
  PhysicalModel(
      shared_ptr<VisualModel> model,
      string visModId,
      string identifier,
      vec3 position,
      bool falling,
      bool collidable,
      float width,
      float height,
      float depth,
      float pitch,
      float yaw,
      float roll,
      float scale);
  string visModId;
  string identifier;
  void draw();
  void move(vec3 direction);
  void move();
  void jump(float speed);
  vec3 position;
  vec3 direction;
  float fallSpeed;
  float moveSpeed;
  float jumpSpeed;
  float targetSpeed;
  float width;
  float height;
  float depth;
  float pitch;
  float yaw;
  float roll;
  float scale;
  bool falling;
  bool collidable;
  friend App;
  friend Controls;
};