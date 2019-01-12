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
  int count = 0;
  EventSubscription sub = GameWindow::addEventListener([&](SDL_Event e){
    std::cout << "kurwa" << std::endl;
    count++;
    if (count > 5 ) sub.unsubscribe();
  });
  GameWindow::startLoop([&]()
  {
    world.draw();
  });
  return 0;
}
