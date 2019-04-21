#include "gameWindow.h"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#define GL_GLEXT_PROTOTYPES 1

std::function<void ()> GameWindow::funcToIterate = [](){};
SDL_Window* GameWindow::window = NULL;
SDL_GLContext GameWindow::context = NULL;
unsigned int GameWindow::callbackCounter = 1;
std::unordered_map<unsigned int, eventCallback> GameWindow::callbacks = {};
std::list<unsigned int> GameWindow::callbacksToRemove = {};

void GameWindow::createWindow(int width, int height)
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  // glDisable(GL_CULL_FACE);
  // int maxVertAttrs;
  // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertAttrs);
  // std::cout << "GL_MAX_VERTEX_ATTRIBS " << maxVertAttrs << std::endl;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, context);
  ImGui_ImplOpenGL3_Init("#version 100");
}
void GameWindow::destroyWindow()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void GameWindow::iterate()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    ImGui_ImplSDL2_ProcessEvent(&e);
    if (e.type == SDL_QUIT)
    {
      destroyWindow();
      exit(0);
    }
    for (auto& callback: callbacks)
    {
      callback.second(e);
    }
    for (unsigned int& id: callbacksToRemove)
    {
      callbacks.erase(id);
    }
    callbacksToRemove.clear();
  }
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();
  funcToIterate();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
}

void GameWindow::startLoop(std::function<void()> func)
{
  funcToIterate = func;
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(iterate, 0, 1);
#else
  while (true)
  {
    iterate();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
#endif
}

EventSubscription GameWindow::addEventListener(eventCallback callback)
{
  unsigned int id = callbackCounter;
  callbackCounter++;
  callbacks.emplace(id, callback);
  return { id };
}

void GameWindow::removeCallback(unsigned int id)
{
  callbacksToRemove.push_back(id);
}

EventSubscription::EventSubscription(unsigned int id):id(id), unsubbed(false) {}

EventSubscription::EventSubscription():id(0), unsubbed(true) {}

void EventSubscription::unsubscribe()
{
  if (unsubbed) return;
  GameWindow::removeCallback(id);
  unsubbed = true;
}