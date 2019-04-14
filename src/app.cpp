#include "app.h"
#include <SDL2/SDL_opengles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

App::App() : camera_vertical_rotation(0.0f),
             camera_horizontal_rotation(0.0f),
             camera_position(0.0f)
{
}

void App::update()
{
  // camera update
  glm::vec3 camPosition;
  glm::vec3 camTarget;
  glm::vec3 camDirection;
  glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
  camDirection.x = sin(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  camDirection.y = sin(glm::radians(camera_vertical_rotation));
  camDirection.z = cos(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  camDirection = glm::normalize(camDirection);
  if (camera_mode == FIRST_PERSON)
  {
    camPosition = camera_position;
    camTarget = camPosition + camDirection;
  }
  else
  {
    camTarget = camera_position;
    camPosition = camTarget + (camDirection * 10.0f);
  }
  glm::mat4 viewMatrix = glm::lookAt(camPosition, camTarget, camUp);
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  for (auto &shader : shaders)
  {
    shader.second->use();
    glUniformMatrix4fv(shader.second->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader.second->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(shader.second->getUniformLocation("ligtPos"), 1, glm::value_ptr(light_position));
    glUniform3fv(shader.second->getUniformLocation("camPos"), 1, glm::value_ptr(camPosition));
  }
  // checking collisions and updating models
  for (auto pair : physical_models)
  {
    auto &model = pair.second;
    vec3 &position = model->position;
    vec3 fall = vec3(0.0f, -1.0f, 0.0f) * model->fallSpeed;
    vec3 jump = vec3(0.0f, 1.0f, 0.0f) * model->jumpSpeed;
    vec3 move = model->direction * model->moveSpeed;
    vec3 movement = fall + jump + move;
    vec3 newPosition = position + movement;
    float left_x = position.x - 1.5f;
    float right_x = position.x + 1.5f;
    float front_z = position.z - 1.5f;
    float back_z = position.z + 1.5f;
    float bottom_y = position.y - 1.5f;
    float top_y = position.y + 1.5f;
    float next_left_x = newPosition.x - 1.5f;
    float next_right_x = newPosition.x + 1.5f;
    float next_front_z = newPosition.z - 1.5f;
    float next_back_z = newPosition.z + 1.5f;
    float next_bottom_y = newPosition.y - 1.5f;
    float next_top_y = newPosition.y + 1.5f;
    bool stopFall = false;
    bool stopJump = false;
    bool stopMove = false;
    for (auto pair2 : physical_models)
    {
      auto &modelToCompare = pair2.second;
      if (model == modelToCompare)
      {
        continue;
      }
      vec3 &comparedPosition = modelToCompare->position;
      float cmpLeftX = comparedPosition.x - 1.5f;
      float cmpRightX = comparedPosition.x + 1.5f;
      float cmpFrontZ = comparedPosition.z - 1.5f;
      float cmpBackZ = comparedPosition.z + 1.5f;
      float cmpBottomY = comparedPosition.y - 1.5f;
      float cmpTopY = comparedPosition.y + 1.5f;
      if (
          (next_left_x <= cmpRightX && next_left_x >= cmpLeftX || next_right_x >= cmpLeftX && next_right_x <= cmpRightX) // x collision
          &&
          (next_top_y >= cmpBottomY && next_top_y <= cmpTopY || next_bottom_y <= cmpTopY && next_bottom_y >= cmpBottomY) // y collision
          &&
          (next_front_z <= cmpBackZ && next_front_z >= cmpFrontZ || next_back_z >= cmpFrontZ && next_back_z <= cmpBackZ) // z collision
      )
      {
        if (bottom_y > cmpTopY)
        {
          stopFall = true;
        }
        else if (top_y < cmpBottomY)
        {
          stopJump = true;
        }
        else
        {
          stopMove = true;
        }
      }
    }
    newPosition = position;
    if (!stopFall)
    {
      newPosition += fall;
    }
    if (!stopJump)
    {
      newPosition += jump;
    }
    if (!stopMove)
    {
      newPosition += move;
    }
    model->update(newPosition, stopMove, stopFall, stopJump);
  }
}

void App::moveCameraStraight(float amount)
{
  glm::vec3 camDirection;
  camDirection.x = sin(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  camDirection.y = sin(glm::radians(camera_vertical_rotation));
  camDirection.z = cos(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  camDirection = glm::normalize(camDirection);
  glm::vec3 movement = camDirection * amount;
  camera_position += movement;
}

void App::moveCameraSideways(float amount)
{
  glm::vec3 camDirection;
  glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
  camDirection.x = sin(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  camDirection.y = sin(glm::radians(camera_vertical_rotation));
  camDirection.z = cos(glm::radians(camera_horizontal_rotation)) * cos(glm::radians(camera_vertical_rotation));
  glm::vec3 right = glm::normalize(glm::cross(camDirection, camUp));
  glm::vec3 movement = right * amount;
  camera_position += movement;
}

void App::moveCameraTo(const vec3 &position)
{
  camera_position = position;
}

void App::moveCameraBy(const vec3 &movement)
{
  camera_position += movement;
}

void App::rotateCameraTo(float hor, float vert)
{
  camera_horizontal_rotation = hor;
  camera_vertical_rotation = vert;
}

void App::rotateCameraBy(float hor, float vert)
{
  camera_horizontal_rotation += hor;
  camera_vertical_rotation += vert;
}

void App::changeCameraMode(CameraMode mode)
{
  camera_mode = mode;
}

void App::addShader(string identifier, string shaderpath)
{
  shared_ptr<Shader> shader(new Shader(shaderpath));
  shaders[identifier] = shader;
  shaderNames.push_back(identifier);
}

void App::addVisualModel(string identifier, string shaderId, string modelpath)
{
  shared_ptr<VisualModel> visModel(new VisualModel(shaders[shaderId], modelpath));
  visual_models[identifier] = visModel;
  visModelNames.push_back(identifier);
}

void App::addVisualModel(string identifier, string shaderId, string modelpath, string texturepath)
{
  shared_ptr<VisualModel> visModel(new VisualModel(shaders[shaderId], modelpath, texturepath));
  visual_models[identifier] = visModel;
  visModelNames.push_back(identifier);
}

void App::addPhysicalModel(string identifier, string visModId, vec3 position, bool static_model)
{
  shared_ptr<PhysicalModel> phModel(new PhysicalModel(visual_models[visModId], position, static_model));
  physical_models[identifier] = phModel;
  phModelNames.push_back(identifier);
}

shared_ptr<PhysicalModel> App::getPhysicalModel(string physModId)
{
  return physical_models[physModId];
}

shared_ptr<VisualModel> App::getVisualModel(string visModId)
{
  return visual_models[visModId];
}

const vector<string>& App::getShaderNames()
{
  return shaderNames;
}

const vector<string>& App::getVisModelNames()
{
  return visModelNames;
}

const vector<string>& App::getPhModelNames()
{
  return phModelNames;
}