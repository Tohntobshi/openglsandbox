#pragma once
#include "visualModel.h"
#include "shader.h"
#include "physicalModel.h"
#include <memory>
#include <unordered_map>

using std::shared_ptr;
using std::unordered_map;
using std::string;
using glm::vec3;

enum CameraMode {
  THIRD_PERSON,
  FIRST_PERSON,
};

class App {
private:
  unordered_map<string, shared_ptr<Shader>> shaders;
  unordered_map<string, shared_ptr<VisualModel>> visual_models;
  unordered_map<string, shared_ptr<PhysicalModel>> physical_models;
  float camera_vertical_rotation;
  float camera_horizontal_rotation;
  vec3 camera_position;
  vec3 light_position;
  CameraMode camera_mode;
public:
  shared_ptr<PhysicalModel> getPhysicalModel(string physModId);
  App();
  void addShader(string identifier, string shaderpath);
  void addVisualModel(string identifier, string shaderId, string modelpath, string texturepath);
  void addVisualModel(string identifier, string shaderId, string modelpath);
  void addPhysicalModel(string identifier, string visModId, vec3 position, bool static_model);
  void moveCameraStraight(float amount);
  void moveCameraSideways(float amount);
  void moveCameraTo(const vec3& position);
  void moveCameraBy(const vec3& movement);
  void rotateCameraTo(float hor, float vert);
  void rotateCameraBy(float hor, float vert);
  void changeCameraMode(CameraMode mode);
  void update();
};