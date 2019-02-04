#include "physicalWorld.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using std::cout;
using std::endl;

void PhysicalWorld::addActiveColidableModel(shared_ptr<PhysicalModel> model)
{
  activeColidableModels.push_back(model);
}

void PhysicalWorld::addPassiveColidableModel(shared_ptr<PhysicalModel> model)
{
  passiveColidableModels.push_back(model);
}

void PhysicalWorld::addNonColidableModel(shared_ptr<PhysicalModel> model)
{
  nonColidableModels.push_back(model);
}

void PhysicalWorld::update()
{
  for (auto model: activeColidableModels)
  {
    vec3& position = model->position;
    vec3 fall = vec3(0.0f,-1.0f, 0.0f) * model->fallSpeed;
    vec3 jump = vec3(0.0f, 1.0f, 0.0f) * model->jumpSpeed;
    vec3 move = model->direction * model->moveSpeed;
    vec3 movement = fall + jump + move;
    vec3 newPosition = position + movement;
    float left_x = position.x - 1.5f;
    float right_x = position.x + 1.5f;
    float front_z = position.z - 1.5f;
    float back_z = position.z + 1.5f;
    float bottom_y = position.y - 1.5f;
    float top_y  = position.y + 1.5f;
    float next_left_x = newPosition.x - 1.5f;
    float next_right_x = newPosition.x + 1.5f;
    float next_front_z = newPosition.z - 1.5f;
    float next_back_z = newPosition.z + 1.5f;
    float next_bottom_y = newPosition.y - 1.5f;
    float next_top_y  = newPosition.y + 1.5f;
    bool stopFall = false;
    bool stopJump = false;
    bool stopMove = false;
    for (auto modelToCompare: activeColidableModels)
    {
      if (model == modelToCompare) { continue; }
      vec3& comparedPosition = modelToCompare->position;
      float cmpLeftX = comparedPosition.x - 1.5f;
      float cmpRightX = comparedPosition.x + 1.5f;
      float cmpFrontZ = comparedPosition.z - 1.5f;
      float cmpBackZ = comparedPosition.z + 1.5f;
      float cmpBottomY = comparedPosition.y - 1.5f;
      float cmpTopY  = comparedPosition.y + 1.5f;
      if (
        (next_left_x <= cmpRightX && next_left_x >= cmpLeftX || next_right_x >= cmpLeftX && next_right_x <= cmpRightX) // x collision
          &&
        (next_top_y >= cmpBottomY && next_top_y <= cmpTopY || next_bottom_y <= cmpTopY && next_bottom_y >= cmpBottomY) // y collision
          &&
        (next_front_z <= cmpBackZ && next_front_z >= cmpFrontZ || next_back_z >= cmpFrontZ && next_back_z <= cmpBackZ) // z collision
      )
      {
        if (bottom_y > cmpTopY) { stopFall = true; }
        else if (top_y < cmpBottomY) { stopJump = true; }
        else { stopMove = true; }
      }
    }
    newPosition = position;
    if (!stopFall) { newPosition += fall; }
    if (!stopJump) { newPosition += jump; }
    if (!stopMove) { newPosition += move; }
    model->update(newPosition, stopMove, stopFall, stopJump);
  }
  for (auto model: passiveColidableModels)
  {
    model->update(vec3(0.0f), false, true, false);
  }
  for (auto model: nonColidableModels)
  {
    model->update(vec3(0.0f), false, true, false);
  }
}