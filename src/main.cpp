#include "gameWindow.h"
#include <iostream>
#include "imgui/imgui.h"
#include "app.h"


using std::shared_ptr;
using glm::vec3;
using std::unordered_map;
using std::string;

void clearGlErrors();
void checkGLErrors();


int main(int argc, char const *argv[])
{
  clearGlErrors();
  GameWindow::createWindow(1024, 768);
  App app;

  app.addShader("strange", "../assets/strange.glsl");
  app.addShader("textured", "../assets/textured.glsl");
  app.addVisualModel("mug", "strange", "../assets/mug.obj");
  app.addVisualModel("donut", "textured", "../assets/donut2.obj", "../assets/donut2.png");
  app.addPhysicalModel("mug0", "mug", vec3(0.0f, 0.0f, 0.0f), false);
  app.addPhysicalModel("mug1", "mug", vec3(3.0f, 2.0f, 0.0f), false);
  app.addPhysicalModel("mug2", "mug", vec3(6.0f, 4.0f, 0.0f), false);
  app.addPhysicalModel("mug3", "mug", vec3(9.0f, 6.0f, 0.0f), false);
  app.addPhysicalModel("mug4", "mug", vec3(12.0f, 9.0f, 0.0f), false);
  app.addPhysicalModel("mug5", "mug", vec3(15.0f, 11.0f, 0.0f), false);
  app.addPhysicalModel("donut", "donut", vec3(0.0f, 10.0f, 0.0f), true);
  auto activeModel = app.getPhysicalModel("donut");

  bool mousepressed = false;
  bool apressed = false;
  bool dpressed = false;
  bool wpressed = false;
  bool spressed = false;
  bool lpressed = false;
  bool ipressed = false;
  bool kpressed = false;
  bool jpressed = false;
  char textinput[128] = "Hello";
  EventSubscription sub = GameWindow::addEventListener([&](SDL_Event e){
    if(e.type == SDL_KEYDOWN) {
      switch(e.key.keysym.scancode)
      {
        case SDL_SCANCODE_A:
          apressed = true;
          break;
        case SDL_SCANCODE_D:
          dpressed = true;
          break;
        case SDL_SCANCODE_W:
          wpressed = true;
          break;
        case SDL_SCANCODE_S:
          spressed = true;
          break;
        case SDL_SCANCODE_F:
          // app.changeCameraMode();
          break;
        case SDL_SCANCODE_T:
          // app.changeCameraMode(THIRD_PERSON);
          break;
        case SDL_SCANCODE_I:
          ipressed = true;
          break;
        case SDL_SCANCODE_K:
          kpressed = true;
          break;
        case SDL_SCANCODE_J:
          jpressed = true;
          break;
        case SDL_SCANCODE_L:
          lpressed = true;
          break;
        case SDL_SCANCODE_SPACE:
          activeModel->jump(1.0);
          break;
      }
    }
     if(e.type == SDL_KEYUP) {
      switch(e.key.keysym.scancode)
      {
        case SDL_SCANCODE_A:
          apressed = false;
          break;
        case SDL_SCANCODE_D:
          dpressed = false;
          break;
        case SDL_SCANCODE_W:
          wpressed = false;
          break;
        case SDL_SCANCODE_S:
          spressed = false;
          break;
        case SDL_SCANCODE_I:
          ipressed = false;
          break;
        case SDL_SCANCODE_K:
          kpressed = false;
          break;
        case SDL_SCANCODE_J:
          jpressed = false;
          break;
        case SDL_SCANCODE_L:
          lpressed = false;
          break;
      }
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
      mousepressed = true;
    }
    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
    {
      mousepressed = false;
    }
    if (e.type == SDL_MOUSEMOTION && mousepressed)
    {
      app.rotateCameraBy(e.motion.xrel, e.motion.yrel);
    }
  });

  
  GameWindow::startLoop([&]()
  {
    if (apressed)
    {
      app.moveCameraSideways(-1.0);
    }
    if (dpressed)
    {
      app.moveCameraSideways(1.0);
    }
    if (wpressed)
    {
      app.moveCameraStraight(1.0f);
    }
    if (spressed)
    {
      app.moveCameraStraight(-1.0f);
    }

    else if (ipressed && lpressed) { activeModel->move(vec3(1.0f, 0.0f, 1.0f)); }
    else if (kpressed && lpressed) { activeModel->move(vec3(-1.0f, 0.0f, 1.0f)); }
    else if (jpressed && kpressed) { activeModel->move(vec3(-1.0f, 0.0f, -1.0f)); }
    else if (ipressed && jpressed) { activeModel->move(vec3(1.0f, 0.0f, -1.0f)); }

    else if (ipressed) { activeModel->move(vec3(1.0f, 0.0f, 0.0f)); }
    else if (kpressed) { activeModel->move(vec3(-1.0f, 0.0f, 0.0f)); }
    else if (jpressed) { activeModel->move(vec3(0.0f, 0.0f, -1.0f)); }
    else if (lpressed) { activeModel->move(vec3(0.0f, 0.0f, 1.0f)); }
    app.update();
    ImGui::Begin("Add model!");
    ImGui::InputText("text", textinput, sizeof(char) * 128);
    if (ImGui::Button("Add")) {
      // add
    }
    ImGui::End();
    checkGLErrors();
  });
  return 0;
}
