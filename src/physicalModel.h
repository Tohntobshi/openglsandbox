#pragma once
#include <memory>
#include "visualModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::shared_ptr;
using glm::vec3;
using glm::mat4;

class App;
class Controls;

class PhysicalModel {
private:
  shared_ptr<VisualModel> visualModel;
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
  void update(vec3 movement, bool dontMove, bool dontFall, bool dontJump);
public:
  PhysicalModel(shared_ptr<VisualModel>, vec3 position, bool falling);
  void move(vec3 direction);
  void move();
  void jump(float speed);
friend App;
friend Controls;
};