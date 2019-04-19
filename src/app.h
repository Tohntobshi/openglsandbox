#pragma once
#include "visualModel.h"
#include "shader.h"
#include "physicalModel.h"
#include <memory>
#include <unordered_map>
#include <vector>

using std::shared_ptr;
using std::unordered_map;
using std::string;
using glm::vec3;
using std::vector;

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
  vector<string> shaderNames;
  vector<string> visModelNames;
  vector<string> phModelNames;
public:
  shared_ptr<PhysicalModel> getPhysicalModel(string physModId);
  shared_ptr<VisualModel> getVisualModel(string visModId);
  App();
  void addShader(string identifier, string shaderpath);
  void addVisualModel(string identifier, string shaderId, string modelpath, string texturepath);
  void addPhysicalModel(
    string identifier,
    string visModId,
    vec3 position,
    bool falling,
    bool collidable,
    float width = 1.0f,
    float height = 1.0f,
    float depth = 1.0f,
    float pitch = 0.0f,
    float yaw = 0.0f,
    float roll = 0.0f,
    float scale = 0.0f
  );
  void moveCameraStraight(float amount);
  void moveCameraSideways(float amount);
  void moveCameraTo(const vec3& position);
  void moveCameraBy(const vec3& movement);
  void rotateCameraTo(float hor, float vert);
  void rotateCameraBy(float hor, float vert);
  void changeCameraMode(CameraMode mode);
  void update();
  const vector<string>& getShaderNames();
  const vector<string>& getVisModelNames();
  const vector<string>& getPhModelNames();
  void saveConfiguration(string filename);
  void loadConfiguration(string filename);
};