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
  Model* model = new Model(shader);
  world.addModel(model);
  world.addShader(shader);
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
  });
  return 0;
}
