#include "app.h"
#include <SDL2/SDL_opengles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>

using std::cout;
using std::ifstream;
using std::ofstream;
using std::runtime_error;

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
  if (shaders.find(identifier) != shaders.end())
  {
    throw runtime_error("identifier " + identifier + " already exists");
  }
  shared_ptr<Shader> shader(new Shader(shaderpath, identifier));
  shaders[identifier] = shader;
  shaderNames.push_back(identifier);
}

void App::addVisualModel(
    string identifier,
    string shaderId,
    string modelpath,
    string texturepath)
{
  if (visual_models.find(identifier) != visual_models.end())
  {
    throw runtime_error("identifier " + identifier + " already exists");
  }
  if (texturepath != "")
  {
    shared_ptr<VisualModel> visModel(new VisualModel(
        shaders[shaderId],
        shaderId,
        modelpath,
        identifier,
        texturepath));
    visual_models[identifier] = visModel;
  }
  else
  {
    shared_ptr<VisualModel> visModel(new VisualModel(
        shaders[shaderId],
        shaderId,
        modelpath,
        identifier));
    visual_models[identifier] = visModel;
  }
  visModelNames.push_back(identifier);
}

void App::addPhysicalModel(
    string identifier,
    string visModId,
    vec3 position,
    bool falling,
    bool collidable,
    float width,
    float height,
    float depth,
    float pitch,
    float yaw,
    float roll,
    float scale)
{
  if (physical_models.find(identifier) != physical_models.end())
  {
    throw runtime_error("identifier " + identifier + " already exists");
  }
  shared_ptr<PhysicalModel> phModel(new PhysicalModel(
      visual_models[visModId],
      visModId,
      identifier,
      position,
      falling,
      collidable,
      width,
      height,
      depth,
      pitch,
      yaw,
      roll,
      scale));
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

const vector<string> &App::getShaderNames()
{
  return shaderNames;
}

const vector<string> &App::getVisModelNames()
{
  return visModelNames;
}

const vector<string> &App::getPhModelNames()
{
  return phModelNames;
}

void App::saveConfiguration(string filename)
{
  ofstream file(filename);
  if (!file.is_open())
  {
    throw runtime_error("cannot open file for writing");
  }
  for (auto &shader : shaders)
  {
    auto &sh = shader.second;
    file << "sh " << sh->identifier << " " << sh->filepath << "\n";
  }
  for (auto &vmodel : visual_models)
  {
    auto &vm = vmodel.second;
    file << "vm " << vm->identifier << " " << vm->shaderId << " " << vm->filepath << " " << vm->texpath << "\n";
  }
  for (auto &phmodel : physical_models)
  {
    auto &pm = phmodel.second;
    file << "pm " << pm->identifier << " " << pm->visModId << " "
         << pm->position.x << " " << pm->position.y << " " << pm->position.z << " "
         << pm->falling << " " << pm->collidable << " "
         << pm->width << " " << pm->height << " " << pm->depth << " "
         << pm->pitch << " " << pm->yaw << " " << pm->roll << " "
         << pm->scale << "\n";
  }
  file.close();
}

void App::loadConfiguration(string filepath)
{
  ifstream file(filepath);
  if (!file.is_open())
  {
    throw runtime_error("Cannot open file " + filepath);
  }
  while (file)
  {
    string line;
    getline(file, line);
    if (line.substr(0, 3) == "sh ")
    {
      char identifier[128] = "";
      char filename[128] = "";
      if (sscanf(line.c_str(), "sh %127s %127s\n", identifier, filename) != 2)
      {
        file.close();
        throw runtime_error("Cannot read file " + filepath);
      }
      try
      {
        addShader(identifier, filename);
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
    }
    else if (line.substr(0, 3) == "vm ")
    {
      char identifier[128] = "";
      char shadername[128] = "";
      char filename[128] = "";
      char texname[128] = "";
      if (sscanf(line.c_str(), "vm %127s %127s %127s %127s\n", identifier, shadername, filename, texname) < 3)
      {
        file.close();
        throw runtime_error("Cannot read file " + filepath);
      }
      try
      {
        addVisualModel(identifier, shadername, filename, texname);
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
    }
    else if (line.substr(0, 3) == "pm ")
    {
      char identifier[128] = "";
      char vismodname[128] = "";
      float x, y, z;
      bool falling, collidable;
      float width, height, depth;
      float pitch, yaw, roll, scale;
      if (sscanf(line.c_str(), "pm %127s %127s %f %f %f %i %i %f %f %f %f %f %f %f\n", identifier, vismodname,
                 &x, &y, &z, &falling, &collidable, &width, &height, &depth, &pitch, &yaw, &roll, &scale) != 14)
      {
        file.close();
        throw runtime_error("Cannot read file " + filepath);
      }
      try
      {
        addPhysicalModel(identifier, vismodname, {x, y, z}, falling, collidable, width, height, depth,
          pitch, yaw, roll, scale);
      }
      catch (runtime_error &e)
      {
        cout << e.what() << "\n";
      }
    }
  }
  file.close();
}