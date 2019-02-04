#include "gameWindow.h"
#include <iostream>
#include "visualModel.h"
#include "camera.h"
#include "physicalWorld.h"
#include "shader.h"
#include <memory>

using std::shared_ptr;
using glm::vec3;

void clearGlErrors();
void checkGLErrors();


int main(int argc, char const *argv[])
{
  clearGlErrors();
  GameWindow::createWindow(640, 480);
  Camera camera;
  shared_ptr<Shader> strangeShader(new Shader("./assets/strange.glsl"));
  shared_ptr<Shader> texturedshader(new Shader("./assets/textured.glsl"));
  shared_ptr<VisualModel> mug(new VisualModel(strangeShader, "./assets/mug.obj"));
  shared_ptr<VisualModel> donut(new VisualModel(texturedshader, "./assets/donut2.obj", "./assets/donut2.png"));
  camera.addShader(strangeShader);
  camera.addShader(texturedshader);
  PhysicalWorld world;
  shared_ptr<PhysicalModel> phMug0(new PhysicalModel(mug, vec3(0.0f, 0.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phMug1(new PhysicalModel(mug, vec3(3.0f, 2.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phMug2(new PhysicalModel(mug, vec3(6.0f, 4.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phMug3(new PhysicalModel(mug, vec3(9.0f, 6.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phMug4(new PhysicalModel(mug, vec3(12.0f, 9.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phMug5(new PhysicalModel(mug, vec3(15.0f, 11.0f, 0.0f), false));
  shared_ptr<PhysicalModel> phDonut(new PhysicalModel(donut, vec3(0.0f, 10.0f, 0.0f), true));
  world.addActiveColidableModel(phDonut);
  world.addActiveColidableModel(phMug0);
  world.addActiveColidableModel(phMug1);
  world.addActiveColidableModel(phMug2);
  world.addActiveColidableModel(phMug3);
  world.addActiveColidableModel(phMug4);
  world.addActiveColidableModel(phMug5);
  bool mousepressed = false;
  bool apressed = false;
  bool dpressed = false;
  bool wpressed = false;
  bool spressed = false;
  bool lpressed = false;
  bool ipressed = false;
  bool kpressed = false;
  bool jpressed = false;
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
          camera.changeCameraMode(FIRST_PERSON);
          camera.updateViewProj();
          break;
        case SDL_SCANCODE_T:
          camera.changeCameraMode(THIRD_PERSON);
          camera.updateViewProj();
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
          phDonut->jump(1.0);
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
      camera.rotateBy(e.motion.xrel, e.motion.yrel);
      camera.updateViewProj();
    }
  });

  
  GameWindow::startLoop([&]()
  {
    if (apressed)
    {
      camera.moveSideways(-1.0);
      camera.updateViewProj();
    }
    if (dpressed)
    {
      camera.moveSideways(1.0);
      camera.updateViewProj();
    }
    if (wpressed)
    {
      camera.moveStraight(1.0f);
      camera.updateViewProj();
    }
    if (spressed)
    {
      camera.moveStraight(-1.0f);
      camera.updateViewProj();
    }

    else if (ipressed && lpressed) { phDonut->move(vec3(1.0f, 0.0f, 1.0f)); }
    else if (kpressed && lpressed) { phDonut->move(vec3(-1.0f, 0.0f, 1.0f)); }
    else if (jpressed && kpressed) { phDonut->move(vec3(-1.0f, 0.0f, -1.0f)); }
    else if (ipressed && jpressed) { phDonut->move(vec3(1.0f, 0.0f, -1.0f)); }

    else if (ipressed) { phDonut->move(vec3(1.0f, 0.0f, 0.0f)); }
    else if (kpressed) { phDonut->move(vec3(-1.0f, 0.0f, 0.0f)); }
    else if (jpressed) { phDonut->move(vec3(0.0f, 0.0f, -1.0f)); }
    else if (lpressed) { phDonut->move(vec3(0.0f, 0.0f, 1.0f)); }
    world.update();
    checkGLErrors();
  });
  return 0;
}
