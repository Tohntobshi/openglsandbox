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
  bool gameMode = false;
  bool mousepressed = false;
  bool apressed = false;
  bool dpressed = false;
  bool wpressed = false;
  bool spressed = false;
  bool lpressed = false;
  bool ipressed = false;
  bool kpressed = false;
  bool jpressed = false;
  bool upressed = false;
  bool opressed = false;
  bool qpressed = false;
  bool zpressed = false;
  bool commapressed = false;
  bool dotpressed = false;
  bool fallCheckbox = false;
  bool collidableCheckbox = false;
  bool activeCheckbox = false;
  bool showActiveModelHitbox = true;
  bool cameraFollowsActiveModel = false;
  bool usePlaneObj = false;
  float backGroundColor[4] = {0.0f,0.0f,0.0f,0.0f};
  char shaderPath[128] = "../assets/";
  char shaderName[128] = "";
  char choosenShader[128] = "";
  char modelPath[128] = "../assets/";
  char texPath[128] = "../assets/";
  char modelName[128] = "";
  char choosenModel[128] = "";
  char phModelName[128] = "";
  char choosenPhModel[128] = "";
  char choosenPhModelToCopy[128] = "";
  char worldPath[128] = "../assets/";
  App& app;
  ImGuiIO& io;
  shared_ptr<PhysicalModel> activeModel;
  shared_ptr<VisualModel> hitboxModel;
  void passInputsToApp();
  void passInputsToAppInGameMode();
  void drawGUI();
public:
  Controls(App& app);
  void processEvent(SDL_Event evt);
  void update();
};