#include "gameWindow.h"
#include <iostream>
#include "imgui/imgui.h"
#include "app.h"
#include "controls.h"

void clearGlErrors();
void checkGLErrors();

int main(int argc, char const *argv[])
{
  clearGlErrors();
  GameWindow::createWindow(1024, 768);
  App app;
  Controls controls(app);
  EventSubscription sub = GameWindow::addEventListener([&](SDL_Event e) {
    controls.processEvent(e);
  });
  GameWindow::startLoop([&]() {
    // ImGui::ShowDemoWindow();
    controls.update();
    // checkGLErrors();
  });
  return 0;
}
