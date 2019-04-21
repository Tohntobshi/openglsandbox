#include "physicalModel.h"
#include <iostream>

using std::cout;
using std::endl;

PhysicalModel::PhysicalModel(
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
  float scale
):
  moveSpeed(0.0f), fallSpeed(0.0f), jumpSpeed(0.0f), targetSpeed(0.0f),
  visualModel(model),
  visModId(visModId),
  identifier(identifier),
  position(position),
  falling(falling),
  collidable(collidable),
  width(width),
  height(height),
  depth(depth),
  pitch(pitch),
  yaw(yaw),
  roll(roll),
  scale(scale)
{
}

void PhysicalModel::draw()
{
  mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale));
  visualModel->bind();
  visualModel->draw(glm::value_ptr(model));
}

void PhysicalModel::move()
{
  targetSpeed = 0.2f;
}

void PhysicalModel::move(vec3 direction)
{
  this->direction = glm::normalize(direction);
  targetSpeed = 0.2f;
}

void PhysicalModel::jump(float speed)
{
  jumpSpeed = speed;
}
