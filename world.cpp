#include "world.h"
#include <SDL2/SDL_opengles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


World::World()
{ 
}

World::~World()
{
  for(Model* object: objects)
  {
    delete object;
  }
  for(Shader* shader: shaders)
  {
    delete shader;
  }
}

void World::addModel(Model* model)
{
  objects.emplace_back(model);
}

void World::addShader(Shader* shader)
{
  shaders.emplace_back(shader);
  updateViewProj();
}

void World::draw()
{
  for(Model* object: objects)
  {
    object->draw();
  }
}

void World::updateViewProj()
{
  float lightPosition[] = { 3.0f, 3.0f, 3.0f };
  glm::vec3 camPosition;
  glm::vec3 camTarget;
  glm::vec3 camDirection;
  glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
  camDirection.x = sin(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  camDirection.y = sin(glm::radians(verticalRotation));
  camDirection.z = cos(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  camDirection = glm::normalize(camDirection);
  if (cameraMode == FIRST_PERSON)
  {
    camPosition = glm::vec3(cameraX, cameraY, cameraZ);
    camTarget = camPosition + camDirection;
  }
  else
  {
    camTarget = glm::vec3(cameraX, cameraY, cameraZ);
    camPosition = camTarget + (camDirection * 10.0f);
  }
  glm::mat4 viewMatrix = glm::lookAt(camPosition, camTarget, camUp);
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  for (auto shader: shaders)
  {
    shader->use();
    glUniformMatrix4fv(shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(shader->getUniformLocation("ligtPos"), 1, lightPosition);
    glUniform3fv(shader->getUniformLocation("camPos"), 1, glm::value_ptr(camPosition));
  }
}

void World::moveBy(float x, float y, float z)
{
  cameraX += x;
  cameraY += y;
  cameraZ += z;
}

void World::rotateBy(float hor, float vert)
{
  horizontalRotation += hor;
  verticalRotation += vert;
}

void World::moveXTo(float x)
{
  cameraX = x;
}

void World::moveYTo(float y)
{
  cameraY = y;
}

void World::moveZTo(float z)
{
  cameraZ = z;
}

void World::rotateHorTo(float deg)
{
  horizontalRotation = deg;
}

void World::rotateVertTo(float deg)
{
  verticalRotation = deg;
}

void World::changeCameraMode(CameraMode mode) {
  cameraMode = mode;
}

void World::moveStraight(float amount)
{
  glm::vec3 camDirection;
  camDirection.x = sin(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  camDirection.y = sin(glm::radians(verticalRotation));
  camDirection.z = cos(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  camDirection = glm::normalize(camDirection);
  glm::vec3 movement = camDirection * amount;
  cameraX += movement.x;
  cameraY += movement.y;
  cameraZ += movement.z;
}

void World::moveSideways(float amount)
{
  glm::vec3 camDirection;
  glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
  camDirection.x = sin(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  camDirection.y = sin(glm::radians(verticalRotation));
  camDirection.z = cos(glm::radians(horizontalRotation)) * cos(glm::radians(verticalRotation));
  glm::vec3 right = glm::normalize(glm::cross(camDirection, camUp));
  glm::vec3 movement = right * amount;
  cameraX += movement.x;
  cameraY += movement.y;
  cameraZ += movement.z;
}