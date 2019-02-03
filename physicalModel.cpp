#include "physicalModel.h"
#include <iostream>

using std::cout;
using std::endl;

PhysicalModel::PhysicalModel(shared_ptr<VisualModel> model, vec3 position)
  : visualModel(model), position(position), speed(0.0f),
    pitch(0.0f), yaw(0.0f), roll(0.0f), scale(1.0f), targetSpeed(0.0f)
{

}

void PhysicalModel::update()
{
  position += direction * speed;
  if (speed < targetSpeed)
  {
    speed += 0.1f;
  }
  else {
    targetSpeed = 0.0f;
    speed = glm::max(speed - 0.1f, 0.0f);
  }
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
  targetSpeed = 1.0f;
}

void PhysicalModel::move(vec3 direction)
{
  this->direction = glm::normalize(direction);
  targetSpeed = 1.0f;
}