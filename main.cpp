#include "gameWindow.h"
#include <iostream>
#include <functional>
#include "world.h"
#include "model.h"
#include "shader.h"


void clearGlErrors();
void checkGLErrors();


int main(int argc, char const *argv[])
{
  clearGlErrors();
  GameWindow::createWindow(640, 480);
  World world;
  Shader* shader = new Shader();
  Model* mug = new Model(shader, "./assets/mug.obj");
  Model* donut = new Model(shader, "./assets/donut.obj");
  world.addModel(mug);
  world.addModel(donut);
  world.addShader(shader);
  mug->addOccurence(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  mug->addOccurence(10.0f, 0.0f, 0.0f, 0.0f, 45.0f, 45.0f, 2.0f);
  donut->addOccurence(-10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  // model->addOccurence(0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  bool mousepressed = false;
  EventSubscription sub = GameWindow::addEventListener([&](SDL_Event e){
    if(e.type == SDL_KEYDOWN) {
      switch(e.key.keysym.scancode)
      {
        case SDL_SCANCODE_A:
          world.moveBy(0.1f, 0.0f, 0.0f);
          world.updateViewProj();
          break;
        case SDL_SCANCODE_D:
          world.moveBy(-0.1f, 0.0f, 0.0f);
          world.updateViewProj();
          break;
        case SDL_SCANCODE_W:
          world.moveBy(0.0f, 0.0f, 1.0f);
          world.updateViewProj();
          break;
        case SDL_SCANCODE_S:
          world.moveBy(0.0f, 0.0f, -1.0f);
          world.updateViewProj();
          break;
        case SDL_SCANCODE_F:
          world.changeCameraMode(FIRST_PERSON);
          world.updateViewProj();
          break;
        case SDL_SCANCODE_T:
          world.changeCameraMode(THIRD_PERSON);
          world.updateViewProj();
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
      world.rotateBy(e.motion.xrel, e.motion.yrel);
      world.updateViewProj();
    }
  });
  GameWindow::startLoop([&]()
  {
    world.draw();
    checkGLErrors();
  });
  return 0;
}
