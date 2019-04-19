#pragma once
#include <SDL2/SDL.h>
#include "app.h"
#include "physicalModel.h"
#include <memory>
#include "imgui/imgui.h"

using std::shared_ptr;
using glm::vec3;
using glm::mat4;
using glm::vec4;

class Controls
{
private:
  bool mousepressed = false;
  bool apressed = false;
  bool dpressed = false;
  bool wpressed = false;
  bool spressed = false;
  bool lpressed = false;
  bool ipressed = false;
  bool kpressed = false;
  bool jpressed = false;
  bool fallCheckbox = false;
  bool collidableCheckbox = false;
  bool showActiveModelHitbox = true;
  char shaderPath[128] = "";
  char shaderName[128] = "";
  char choosenShader[128] = "";
  char modelPath[128] = "";
  char texPath[128] = "";
  char modelName[128] = "";
  char choosenModel[128] = "";
  char phModelName[128] = "";
  char choosenPhModel[128] = "";
  char worldPath[128] = "";
  App& app;
  ImGuiIO& io;
  shared_ptr<PhysicalModel> activeModel;
  shared_ptr<VisualModel> hitboxModel;
public:
  Controls(App& app);
  void processEvent(SDL_Event evt);
  void update();
  Controls& operator=(const Controls& co) = delete;
};